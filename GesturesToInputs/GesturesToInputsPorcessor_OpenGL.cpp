#include "GesturesToInputsPorcessor_OpenGL.h"

#include <iostream>
#include <opencv2\highgui.hpp>

#include "Gesture.h"

namespace GesturesToInputs {

    static void error_callback(int error, const char* description) {
        fprintf(stderr, "GLFW Error: %s\n", description);
    }

    static void messageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        auto self = static_cast<GesturesToInputsPorcessor_OpenGL*>(const_cast<void*>(userParam));
        self->opengl_has_errored = true;
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
    }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    GesturesToInputsPorcessor_OpenGL::GesturesToInputsPorcessor_OpenGL(
        MediaFoundationWebcam* webcam,
        std::vector<ThresholdData>* thresholdData,
        std::vector<TrackerData>* trackers,
        std::vector<GestureRuleData>* rules,
        std::vector<GestureInput>* gestures
    )
        : GesturesToInputsProcessor{ gestures },
        webcam{ webcam },
        sourceWidth{ webcam->getWidth() },
        sourceHeight{ webcam->getHeight() }
    {
        if (!glfwInit())
        {
            // Initialization failed
            throw "GLFW could not initialise OpenGL";
        }

        glfwSetErrorCallback(error_callback);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_CONTEXT_ROBUSTNESS, GLFW_LOSE_CONTEXT_ON_RESET);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(sourceWidth, sourceHeight, "Tracked Objects", NULL, NULL);
        if (!window)
        {
            throw "GLFW could not create a window";
        }

        glfwSetKeyCallback(window, key_callback);
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            throw "Could not set up GLAD";
        }

        glDebugMessageCallback(messageCallback, this);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_FALSE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, NULL, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL, GL_TRUE);

        glfwSwapInterval(0);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        int work_group_size[3];
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);
        xMaxInstances = work_group_size[0];
        yMaxInstances = work_group_size[1];
        zMaxInstances = work_group_size[2];
        glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &totalMaxInstances);

        std::cout << std::endl << "(" << xMaxInstances << ", " << yMaxInstances << ", " << zMaxInstances << ") = " << totalMaxInstances << std::endl;

        int maxUnits;
        glGetIntegerv(GL_MAX_IMAGE_UNITS, &maxUnits);
        int maxComputeUnits;
        glGetIntegerv(GL_MAX_COMPUTE_IMAGE_UNIFORMS, &maxComputeUnits);
        std::cout << "Units " << maxUnits << " & Uniforms " << maxComputeUnits << std::endl;
        
        bindImageHandle(&inputTextureHandle, inputTextureUnit, INPUT_IMAGE_UNIT);
        convertYUY2ToRGB();
        convertToHSV();
        threshold(thresholdData);
        searchForObjects(trackers);
        detectGesturesSetup(gestures->size(), rules);
        displayOutputSetup();
    }

    GesturesToInputsPorcessor_OpenGL::~GesturesToInputsPorcessor_OpenGL()
    {
        GLuint textures[] = { inputTextureHandle, thresholdTextureHandle, outputTextureHandle, rawDataTextureHandle };
        glDeleteTextures(sizeof(textures) / sizeof(textures[0]), &textures[0]);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        GLuint buffers[] = { thresholdShaderBuffer, trackerShaderBuffer, computeShaderBuffer, gestureFoundShaderBuffer, gestureRuleShaderBuffer, vertex_buffer, element_buffer };
        glDeleteBuffers(sizeof(buffers) / sizeof(buffers[0]), &buffers[0]);

        glDeleteVertexArrays(1, &vertex_array);

        glDeleteProgram(hsvShader.getHandle());
        glDeleteProgram(thresholdShader.getHandle());
        glDeleteProgram(objectSearchShader.getHandle());
        glDeleteProgram(detectGesturesShader.getHandle());
        glDeleteProgram(displayShader.getHandle());
        glDeleteProgram(yuy2Shader.getHandle());

        glfwDestroyWindow(window);
        glfwTerminate();
    }
    
    void GesturesToInputsPorcessor_OpenGL::run()
    {
        auto gesture = Gesture::getInstance();
        PerformanceTimer perf;

        // TODO:: Allow more formats. If YUY2 it can be converted, if it's a format GLTexSubImage2D knows about it can be uploaded directly
        while (!glfwWindowShouldClose(window) && !opengl_has_errored)
        {
            webcam->wait();
            std::cout << "DEAD TIME "; perf.End();
            perf.Start();
            auto bytes = webcam->getData();
            glActiveTexture(rawDataTextureUnit);
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sourceWidth, sourceHeight, GL_RG_INTEGER, GL_UNSIGNED_BYTE, bytes);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            yuy2Shader.compute();
            hsvShader.compute();
            thresholdShader.compute();
            //debugDisplayTexture(thresholdTextureUnit, "threshold");
            objectSearchShader.compute();
            detectGesturesShader.compute();

            gesture->reset();
            for (unsigned int i = 0; i < gestureDetection->getGestures()->size(); i++) {
                gesture->handleInput(&gestureDetection->getGestures()->at(i), gestureFoundDataPointer[i] == 1);
            }
            gesture->complete();

            std::cout << "Processed: ";  perf.End(); std::cout << std::endl;

            perf.Start();
            displayOutput();
            std::cout << "Output displayed in "; perf.End();
            perf.Start();
            std::cout << std::endl << std::endl << std::endl;

            glfwPollEvents();
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, gestureFoundShaderBuffer);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        webcam->Close();
        webcam->Release();

    }

    void GesturesToInputsPorcessor_OpenGL::convertYUY2ToRGB() {
        yuy2Shader.compileCompute("Convert_YUY2.comp", sourceWidth / 2, sourceHeight, 1);
        yuy2Shader.setUniform("inputImage", INPUT_IMAGE_UNIT);
        yuy2Shader.setUniform("rawData", RAW_DATA_IMAGE_UNIT);
        bindImageHandle(&rawDataTextureHandle, rawDataTextureUnit, RAW_DATA_IMAGE_UNIT, GL_RG8UI, GL_READ_ONLY);
    }

    void GesturesToInputsPorcessor_OpenGL::convertToHSV() {
        hsvShader.compileCompute("ConvertToHSV.comp", sourceWidth, sourceHeight, 1);

        hsvShader.setUniform("inputImage", INPUT_IMAGE_UNIT);
        hsvShader.setUniform("thresholdTexture", THRESHOLD_IMAGE_UNIT);
        hsvShader.setUniform("outputImage", OUTPUT_IMAGE_UNIT);
        hsvShader.setUniform("inputIsMirrored", unsigned int(1));
        hsvShader.setUniform3("rgbCorrections", new float[3]{ 0.0f, 0.05f, 0.0f });
        hsvShader.setUniform("lightenAmount", 0.5f);

        bindImageHandle(&thresholdTextureHandle, thresholdTextureUnit, THRESHOLD_IMAGE_UNIT);
        bindImageHandle(&outputTextureHandle, outputTextureUnit, OUTPUT_IMAGE_UNIT);
    }

    void GesturesToInputsPorcessor_OpenGL::threshold(std::vector<ThresholdData>* items) {
        thresholdShader.compileCompute("Threshold.comp", sourceWidth, sourceHeight, 1);
        thresholdShader.setUniform("numberOfColours", unsigned int(items->size()));
        thresholdShader.setUniform("thresholdTexture", THRESHOLD_IMAGE_UNIT);
        createStorageBuffer(&thresholdShaderBuffer, items, SHADER_STORAGE_THRESHOLD);
    }

    void GesturesToInputsPorcessor_OpenGL::searchForObjects(std::vector<TrackerData>* trackers) {
        objectSearchShader.compileCompute("ObjectBoundingBoxSearch_Pass1.comp", sampleColumns, sampleRows, trackers->size());

        objectSearchShader.setUniform("thresholdTexture", THRESHOLD_IMAGE_UNIT);

        objectSearchShader.setUniform2("samplePixelDimensions", new unsigned int[2]{ sourceWidth / sampleColumns, sourceHeight / sampleRows });
        objectSearchShader.setUniform2("sampleSize", new unsigned int[2]{ sampleColumns, sampleRows });
        objectSearchShader.setUniform("numberOfPasses", unsigned int(100));
        objectSearchShader.setUniform("threshold", unsigned int(500));
        objectSearchShader.setUniform("outputImage", OUTPUT_IMAGE_UNIT);

        createEmptyStorageBuffer(&computeShaderBuffer, sizeof(ObjectSearchData) * totalSamples, SHADER_STORAGE_OBJECT_SEARCH);
        createStorageBuffer(&trackerShaderBuffer, trackers, SHADER_STORAGE_TRACKERS);
    }

    void GesturesToInputsPorcessor_OpenGL::detectGesturesSetup(unsigned int numberOfGestures, std::vector<GestureRuleData>* rules) {
        detectGesturesShader.compileCompute("DetectGestures.comp", rules->size(), 1, 1);
        gestureFoundDataPointer = createEmptyPersistantStorageBuffer(&gestureFoundShaderBuffer, numberOfGestures * sizeof(unsigned int), SHADER_STORAGE_GESTURE);
        createStorageBuffer(&gestureRuleShaderBuffer, rules, SHADER_STORAGE_GESTURE_RULE);
    }

    void GesturesToInputsPorcessor_OpenGL::displayOutputSetup() {
        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &element_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        displayShader.addShader(GL_VERTEX_SHADER, "test.vert");
        displayShader.addShader(GL_FRAGMENT_SHADER, "test.frag");
        displayShader.compile();
        displayShader.use();

        vpos_location = displayShader.attributeLocation("aPosition");
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);

        tex_location = displayShader.attributeLocation("aTexCoord");
        glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(2 * sizeof(float)));

        displayShader.setUniform("outputImage", OUTPUT_IMAGE_UNIT);

        glViewport(0, 0, sourceWidth, sourceHeight);
    }

    void GesturesToInputsPorcessor_OpenGL::displayOutput() {
        displayShader.use();
        glEnableVertexAttribArray(vpos_location);
        glEnableVertexAttribArray(tex_location);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        glDisableVertexAttribArray(vpos_location);
        glDisableVertexAttribArray(tex_location);
        glfwSwapBuffers(window);
    }

    void GesturesToInputsPorcessor_OpenGL::debugDisplayTexture(GLenum textureUnit, std::string windowName) {
        glActiveTexture(textureUnit);
        unsigned char* gl_texture_bytes = (unsigned char*)malloc(sizeof(unsigned char) * sourceWidth * sourceHeight * 3);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, gl_texture_bytes);
        // TODO:: use OpenGL to display this.
        cv::imshow(windowName, cv::Mat(sourceHeight, sourceWidth, CV_8UC3, gl_texture_bytes));
        free(gl_texture_bytes);
    }

    template<typename T>void GesturesToInputsPorcessor_OpenGL::createStorageBuffer(GLuint* bufferHandle, std::vector<T>* items, int binding) {
        glGenBuffers(1, bufferHandle);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *bufferHandle);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, *bufferHandle);

        uint dataSize = sizeof(T) * items->size();
        T* bufferItems;
        bufferItems = (T*)malloc(dataSize);
        int i = 0;
        for (auto& item : *items) { bufferItems[i] = item; i++; }
        glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, bufferItems, GL_STATIC_DRAW);
        free(bufferItems);

    }

    void GesturesToInputsPorcessor_OpenGL::createEmptyStorageBuffer(GLuint* bufferHandle, unsigned int dataSize, int binding) {
        glGenBuffers(1, bufferHandle);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *bufferHandle);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, *bufferHandle);
        glBufferData(GL_SHADER_STORAGE_BUFFER, dataSize, NULL, GL_STATIC_COPY);
    }

    unsigned int* GesturesToInputsPorcessor_OpenGL::createEmptyPersistantStorageBuffer(GLuint* bufferHandle, unsigned int dataSize, int binding) {
        auto flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT;
        glGenBuffers(1, bufferHandle);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, *bufferHandle);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, *bufferHandle);
        glBufferStorage(GL_SHADER_STORAGE_BUFFER, dataSize, NULL, flags);
        return (unsigned int*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, dataSize, flags);
    }

    void GesturesToInputsPorcessor_OpenGL::bindImageHandle(GLuint* handle, GLenum textureUnit, int binding, int format, int access) {
        glActiveTexture(textureUnit);
        glGenTextures(1, handle);
        glBindTexture(GL_TEXTURE_2D, *handle);
        glTexStorage2D(GL_TEXTURE_2D, 1, format, sourceWidth, sourceHeight);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindImageTexture(binding, *handle, 0, GL_FALSE, 0, access, format);
    }
}
