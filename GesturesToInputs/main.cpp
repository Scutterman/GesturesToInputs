#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "glfw/include/GLFW/glfw3.h"

#include "main.h"

#include <iostream>
#include <filesystem>

#include "GesturesToInputs.h"
#include "Shader.h"

#include <opencv2/highgui.hpp>

using namespace GesturesToInputs;

std::list<GestureInput> dragonGameGestures() {
    auto boostRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::CENTRE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::CENTRE)
    };

    auto flySlowlyRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::FAR_LEFT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    auto upRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::TOP),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (int)VERTICAL_POSITION::TOP)
    };

    auto tiltLeftRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::LESS_THAN, "Green")
    };

    auto downRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::BOTTOM),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (int)VERTICAL_POSITION::BOTTOM)
    };

    auto tiltRightRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::MORE_THAN, "Green")
    };

    return std::list<GestureInput>{
        GestureInput(boostRules, DIK_F, "BOOST"),
        GestureInput(flySlowlyRules, DIK_SPACE, "SLOW DOWN"),
        GestureInput(upRules, DIK_W, "FLY UP"),
        GestureInput(tiltLeftRules, DIK_A, "TILT LEFT"),
        GestureInput(downRules, DIK_S, "FLY DOWN"),
        GestureInput(tiltRightRules, DIK_D, "TILT RIGHT"),
    };
}

std::list<GestureInput> justCause2Gestures() {
    auto grappleRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::CENTRE)
    };

    auto jumpRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::CENTRE)
    };

    auto primaryWeaponRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::FAR_LEFT)
    };
    
    auto secondaryWeaponRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    auto forwardRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::TOP)
    };

    auto leftRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::FAR_LEFT)
    };

    auto backwardRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::BOTTOM),
    };

    auto rightRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (int)HORIZONTAL_POSITION::CENTRE)
    };

    auto lookUpRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (int)VERTICAL_POSITION::TOP)
    };

    auto lookLeftRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::CENTRE)
    };

    auto lookDownRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (int)VERTICAL_POSITION::BOTTOM),
    };

    auto lookRightRules = std::list<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    return std::list<GestureInput>{
        GestureInput(grappleRules, DIK_F, "GRAPPLE"),
        GestureInput(jumpRules, DIK_SPACE, "JUMP"),
        GestureInput(primaryWeaponRules, MOUSEEVENTF_LEFTDOWN, "FIRE 1", GESTURE_INPUT_TYPE::MOUSE_BUTTON),
        GestureInput(secondaryWeaponRules, MOUSEEVENTF_RIGHTDOWN, "FIRE 2", GESTURE_INPUT_TYPE::MOUSE_BUTTON),
        GestureInput(lookUpRules, (int)MOUSE_DIRECTION::UP, "LOOK UP", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(lookLeftRules, (int)MOUSE_DIRECTION::LEFT, "LOOK LEFT", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(lookDownRules, (int)MOUSE_DIRECTION::DOWN, "LOOK DOWN", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(lookRightRules, (int)MOUSE_DIRECTION::RIGHT, "LOOK RIGHT", GESTURE_INPUT_TYPE::MOUSE_MOVE),
        GestureInput(forwardRules, DIK_W, "FORWARD"),
        GestureInput(leftRules, DIK_A, "LEFT"),
        GestureInput(backwardRules, DIK_S, "BACK"),
        GestureInput(rightRules, DIK_D, "RIGHT"),
    };
}

GLFWwindow* window;

void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int end(std::string message = "") {
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << message << '\n';
    return 1;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

struct ThresholdData {
    float lowColour[4];
    float highColour[4];
    float trackerColour[4];

    ThresholdData(float _lowColour[4], float _highColour[4], float _trackerColour[4]) {
        lowColour[0] = _lowColour[0];
        lowColour[1] = _lowColour[1];
        lowColour[2] = _lowColour[2];
        lowColour[3] = _lowColour[3];
        highColour[0] = _highColour[0];
        highColour[1] = _highColour[1];
        highColour[2] = _highColour[2];
        highColour[3] = _highColour[3];
        trackerColour[0] = _trackerColour[0];
        trackerColour[1] = _trackerColour[1];
        trackerColour[2] = _trackerColour[2];
        trackerColour[3] = _trackerColour[3];
    }
};

struct ObjectSearchData {
    GLint boundingBox[4];
    GLuint area;
    GLuint topLeftSampleIndex;
    GLuint isPartOfAnObject;
    GLuint isObjectTopLeft;
};

static const struct
{
    float x, y;
    float xTexture, yTexture;
} vertices[4] =
{
    { -1.0f,  1.0f, 0.0f, 1.0f },
    { -1.0f, -1.0f, 0.0f, 0.0f },
    {  1.0f, -1.0f, 1.0f, 0.0f },
    {  1.0f,  1.0f, 1.0f, 1.0f },
};

const unsigned int indices[] = { 0, 1, 2, 3, 0, 2 };
const int INPUT_IMAGE_UNIT = 0;
const int OUTPUT_IMAGE_UNIT = 0;
const int THRESHOLD_IMAGE_UNIT = 1;

const int SHADER_STORAGE_THRESHOLD = 0;
const int SHADER_STORAGE_OBJECT_SEARCH = 0;

const unsigned int sampleColumns = 64;
const unsigned int sampleRows = 48;
const unsigned int totalSamples = sampleColumns * sampleRows;

bool opengl_has_errored = false;
GLuint frameTextureHandle, thresholdTextureHandle, outputImageHandle;
const GLenum inputTextureUnit = GL_TEXTURE0;
const GLenum thresholdTextureUnit = GL_TEXTURE1;
const GLenum outputTextureUnit = GL_TEXTURE2;

void checkError(std::string stage) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        opengl_has_errored = true;
        std::string error;

        switch (err) {
        case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
        case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
        case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
        case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
        default: error = "No message programmed";
        }
        std::cout << "error at " << stage << ": " << err << "-" << error << '\n';
    }
}

int setup(int width, int height) {
    // TODO:: If no GPU or opengl version < 4.3 then fall back to cpu method.
    // TODO:: Cpu method may be optimisable since we only need rough bounding box. Moments can certainly be removed, and perhaps more optimisaitons.
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
    {
        // Initialization failed
        return end("GLFW could not initialise OpenGL");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Output", NULL, NULL);
    if (!window)
    {
        return end("GLFW could not create a window");
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return end("Could not set up GLAD");
    }
    
    glfwSwapInterval(1);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    checkError("clear colour");

    return 0;
}

void bindOpenCVImage(GLuint* handle, GLenum textureUnit, cv::Mat* source) {
    glActiveTexture(textureUnit);
    glGenTextures(1, handle);
    glBindTexture(GL_TEXTURE_2D, GLuint(*handle));
    checkError("bind texture");
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, cv::Mat(*source).cols, cv::Mat(*source).rows);
    checkError("texture storage");
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkError("texture options");
    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, cv::Mat(*source).cols, cv::Mat(*source).rows, GL_BGR, GL_UNSIGNED_BYTE, cv::Mat(*source).ptr());
    checkError("sending data");
}

void bindImageHandle(GLuint* handle, GLenum textureUnit, int width, int height) {
    glActiveTexture(textureUnit);
    glGenTextures(1, handle);
    glBindTexture(GL_TEXTURE_2D, GLuint(*handle));
    checkError("generating textures");
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, width, height);
    checkError("texture storage");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkError("texture options");
}

void convertToHSV(std::filesystem::path basePath, cv::Mat* inputImage) {
    Shader hsvShader;
    hsvShader.addShader(GL_COMPUTE_SHADER, (basePath / "shaders" / "ConvertToHSV.comp").string());
    checkError("hsv shader");
    hsvShader.compile();
    checkError("hsv shader compile");
    hsvShader.use();

    auto inputTextureLocation = hsvShader.uniformLocation("inputImage");
    checkError("Get Input Texture Location");
    
    bindOpenCVImage(&frameTextureHandle, inputTextureUnit, inputImage);
    glBindImageTexture(INPUT_IMAGE_UNIT, frameTextureHandle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    checkError("Bind input image");
    glUniform1i(inputTextureLocation, INPUT_IMAGE_UNIT);
    checkError("Set Texture Location");

    auto thresholdTextureLocation = hsvShader.uniformLocation("thresholdTexture");
    checkError("Get Threshold Texture Location");
    
    bindImageHandle(&thresholdTextureHandle, thresholdTextureUnit, cv::Mat(*inputImage).cols, cv::Mat(*inputImage).rows);
    glBindImageTexture(THRESHOLD_IMAGE_UNIT, thresholdTextureHandle, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    checkError("Bind Threshold Image");
    glUniform1i(thresholdTextureLocation, THRESHOLD_IMAGE_UNIT);
    checkError("Set Texture Location");

    glDispatchCompute(cv::Mat(*inputImage).cols, cv::Mat(*inputImage).rows, 1);
    checkError("After Shader");
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    checkError("After Barrier");
}

void threshold(std::filesystem::path basePath, int width, int height, std::vector<ThresholdData> items) {
    Shader thresholdShader;
    thresholdShader.addShader(GL_COMPUTE_SHADER, (basePath / "shaders" / "Threshold.comp").string());
    checkError("threshold shader");
    thresholdShader.compile();
    checkError("threshold shader compile");
    thresholdShader.use();

    auto thresholdTextureLocation = thresholdShader.uniformLocation("thresholdTexture");
    checkError("Get Threshold Texture Location");
    glUniform1i(thresholdTextureLocation, THRESHOLD_IMAGE_UNIT);
    checkError("Set Texture Location");

    GLuint thresholdShaderBuffer;
    glGenBuffers(1, &thresholdShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, thresholdShaderBuffer);

    uint thresholdDataSize = sizeof(ThresholdData) * items.size();
    ThresholdData* bufferItems;
    bufferItems = (ThresholdData*)malloc(thresholdDataSize);
    int i = 0;
    for (auto& item : items) { bufferItems[i] = item; i++; }
    glBufferData(GL_SHADER_STORAGE_BUFFER, thresholdDataSize, bufferItems, GL_STATIC_COPY);
    free(bufferItems);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_THRESHOLD, thresholdShaderBuffer);
    checkError("After Buffer");

    glDispatchCompute(width, height, items.size());
    checkError("After Shader");
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    checkError("After Barrier");
}

void searchForObjects(std::filesystem::path basePath, int width, int height) {

    //auto redTrackerValues = std::list<TrackerValues>{ TrackerValues(169, 10, 104, 255, 151, 255) };
    //auto redTracker = Tracker("Red", redTrackerValues, cv::Scalar(0, 0, 255));
    //cv::Mat threshold;
    //threshold = redTracker.isolateColours(source);

    Shader objectSearchShader;
    objectSearchShader.addShader(GL_COMPUTE_SHADER, (basePath / "shaders" / "ObjectBoundingBoxSearch_Pass1.comp").string());
    checkError("object search shader");
    objectSearchShader.compile();
    checkError("object search shader compile");
    objectSearchShader.use();

    auto objectTextureLocation = objectSearchShader.uniformLocation("thresholdTexture");
    checkError("Get Threshold Texture Location");
    glUniform1i(objectTextureLocation, THRESHOLD_IMAGE_UNIT);
    checkError("Set Threshold Texture Location");

    GLint samplePixelDimensions_location, sampleSize_location, passes_location, threshold_location;
    samplePixelDimensions_location = objectSearchShader.uniformLocation("samplePixelDimensions");
    sampleSize_location = objectSearchShader.uniformLocation("sampleSize");
    passes_location = objectSearchShader.uniformLocation("numberOfPasses");
    threshold_location = objectSearchShader.uniformLocation("threshold");

    int samplePixelDimensions[2];
    samplePixelDimensions[0] = width / sampleColumns;
    samplePixelDimensions[1] = height / sampleRows;
    glUniform2iv(samplePixelDimensions_location, 1, samplePixelDimensions);

    int sampleSize[2];
    sampleSize[0] = sampleColumns;
    sampleSize[1] = sampleRows;
    glUniform2iv(sampleSize_location, 1, sampleSize);

    unsigned int numberOfPasses = 100;
    glUniform1ui(passes_location, numberOfPasses);

    unsigned int thresholdArea = 2;
    glUniform1ui(threshold_location, thresholdArea);

    uint objectDataSize = sizeof(ObjectSearchData) * totalSamples;

    GLuint computeShaderBuffer;
    glGenBuffers(1, &computeShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, objectDataSize, NULL, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_OBJECT_SEARCH, computeShaderBuffer);

    checkError("After Buffer");

    glDispatchCompute(sampleColumns, sampleRows, 1);
    checkError("After Shader");
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    checkError("After Barrier");

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
}

void debugBoundingBoxes(cv::Mat* source) {
    auto objectBufferData = (ObjectSearchData*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    // TODO:: Apparently the first 64 samples are all top left of an object, and nothing else is.
    // Also, three of the bounding box values seem to be uninitialised
    int columns = cv::Mat(*source).cols;
    int rows = cv::Mat(*source).rows;
    int samplePixelColumns = columns / sampleColumns;
    int samplePixelRows = rows / sampleRows;
    for (int i = 0; i < totalSamples; i++) {
        if (objectBufferData[i].isObjectTopLeft == 1) {
            std::cout << i << " (" << objectBufferData[i].topLeftSampleIndex << ") " << " is part of object? " << objectBufferData[i].isPartOfAnObject << std::endl;
            std::cout << objectBufferData[i].boundingBox[0] * samplePixelColumns << "," << objectBufferData[i].boundingBox[1] * samplePixelRows << " to ";
            std::cout << objectBufferData[i].boundingBox[2] * samplePixelColumns << "," << objectBufferData[i].boundingBox[3] * samplePixelRows;
            std::cout << " (" << objectBufferData[i].area << " pixels)" << std::endl;

            auto x = objectBufferData[i].boundingBox[0] * samplePixelColumns;
            auto y = objectBufferData[i].boundingBox[1] * samplePixelRows;
            auto width = (objectBufferData[i].boundingBox[2] * samplePixelColumns) - x;
            auto height = (objectBufferData[i].boundingBox[3] * samplePixelRows) - y;
            cv::rectangle(cv::Mat(*source), cv::Rect(x, y, width, height), cv::Scalar(0, 255, 0, 255));
        }
    }

    cv::imshow("Source with lines", cv::Mat(*source));

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void convertToRGB(std::filesystem::path basePath, int width, int height) {
    Shader rgbShader;
    rgbShader.addShader(GL_COMPUTE_SHADER, (basePath / "shaders" / "ConvertToRGB.comp").string());
    checkError("rgb shader");
    rgbShader.compile();
    checkError("rgb shader compile");
    rgbShader.use();

    auto thresholdTextureLocation = rgbShader.uniformLocation("thresholdTexture");
    checkError("Get Threshold Texture Location");

    checkError("Bind Threshold Image");
    glUniform1i(thresholdTextureLocation, THRESHOLD_IMAGE_UNIT);
    checkError("Set Texture Location");

    auto outputImageTextureLocation = rgbShader.uniformLocation("outputImage");
    checkError("Get Input Texture Location");

    bindImageHandle(&outputImageHandle, outputTextureUnit, width, height);
    glBindImageTexture(OUTPUT_IMAGE_UNIT, outputImageHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    checkError("Bind output image");
    glUniform1i(outputImageTextureLocation, OUTPUT_IMAGE_UNIT);
    checkError("Set Texture Location");
    
    glDispatchCompute(width, height, 1);
    checkError("After Shader");
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
    checkError("After Barrier");
}

void displayOutput(std::filesystem::path basePath) {
    GLuint vertex_array, vertex_buffer, element_buffer;
    GLint vpos_location, tex_location, texture0Height_location;
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    checkError("vertex buffer");

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    checkError("element buffer");

    Shader shader;
    bool vertexShaderSuccess, fragmentShaderSuccess, shaderCompileSuccess;
    std::filesystem::path vertexPath = basePath / "shaders" / "test.vert", fragmentPath = basePath / "shaders" / "test.frag";

    vertexShaderSuccess = shader.addShader(GL_VERTEX_SHADER, vertexPath.string());
    checkError("vertex shader");

    fragmentShaderSuccess = shader.addShader(GL_FRAGMENT_SHADER, fragmentPath.string());
    checkError("fragment shader");

    shaderCompileSuccess = shader.compile();
    checkError("program");

    shader.use();
    checkError("use program 1");

    vpos_location = shader.attributeLocation("aPosition");
    checkError("get vertex location");

    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vpos_location);
    checkError("vertex attribute");

    tex_location = shader.attributeLocation("aTexCoord");
    checkError("get texture location");

    glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(tex_location);
    checkError("texture attribute");

    texture0Height_location = shader.uniformLocation("texture0Height");
    checkError("get height location");
    glUniform1i(texture0Height_location, height);

    glViewport(0, 0, width, height);
    checkError("viewport size");
    glClear(GL_COLOR_BUFFER_BIT);
    checkError("clear");

    checkError("use program");
    checkError("bind texture");
    glBindVertexArray(vertex_array);
    checkError("bind array");
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    checkError("draw");

    glfwSwapBuffers(window);
}

void debugDisplayTexture(GLuint* handle, int width, int height, std::string windowName) {
    glBindTexture(GL_TEXTURE_2D, GLuint(*handle));
    checkError(windowName + " display texture bind");
    unsigned char* gl_texture_bytes = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 3);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, gl_texture_bytes);
    checkError(windowName + " display texture get");
    cv::imshow(windowName, cv::Mat(height, width, CV_8UC3, gl_texture_bytes));
    free(gl_texture_bytes);
}

int main(int argc, char** argv)
{
    auto cam = Webcam();
    auto frame = cam.next();
    auto dir = std::filesystem::path(argv[0]).parent_path();
    
    int status = setup(frame.source.size().width, frame.source.size().height);
    if (status != 0) {
        return status;
    }

    std::vector<ThresholdData> trackers;
    float low[4] = { 80, 111, 110, 255 };
    float high[4] = { 95, 255, 255, 255 };
    float tracker[4] = { 87, 183, 183, 255 };
    trackers.push_back(ThresholdData(low, high, tracker));

    /*
    PerformanceTimer perf;
    perf.Start();
    */
    convertToHSV(dir, &(frame.source));
    debugDisplayTexture(&thresholdTextureHandle, frame.source.cols, frame.source.rows, "HSV IMAGE");
    threshold(dir, frame.source.cols, frame.source.rows, trackers);
    debugDisplayTexture(&thresholdTextureHandle, frame.source.cols, frame.source.rows, "Threshold image");
    convertToRGB(dir, frame.source.cols, frame.source.rows);
    debugDisplayTexture(&frameTextureHandle, frame.source.cols, frame.source.rows, "SOURCE IMAGE");
    debugDisplayTexture(&outputImageHandle, frame.source.cols, frame.source.rows, "RGB OUTPUT");
    /*
    debugDisplayTexture(&outputImageHandle, frame.source.cols, frame.source.rows, "HSV IMAGE TO RGB");*/
    /*
    threshold(dir, frame.source.cols, frame.source.rows, trackers);
    convertToRGB(dir, frame.source.cols, frame.source.rows);
    searchForObjects(dir, frame.source.cols, frame.source.rows);
    perf.End();
    */
    
    //debugBoundingBoxes(&(frame.source));
    while (!glfwWindowShouldClose(window) && !opengl_has_errored)
    {
        glfwPollEvents();
    }
    
    return end();

    /*try {
        auto redTrackerValues = std::list<TrackerValues> { TrackerValues(169, 10, 104, 255, 151, 255) };
        auto greenTrackerValues = std::list<TrackerValues> { TrackerValues(80, 95, 111, 255, 110, 255) };
        
        std::map<std::string, Tracker> trackers = {
            { "Red", Tracker("Red", redTrackerValues, cv::Scalar(0, 0, 255)) },
            { "Green", Tracker("Green", greenTrackerValues, cv::Scalar(0, 255, 0)) }
        };

        auto processor = GesturesToInputsProcessor(trackers, justCause2Gestures());
        processor.webcamIndex = 0;
        processor.webcamMirrored = true;
        processor.run();
    }
    catch (int e) {
        std::cout << "An exception occurred. Exception Nr. " << e << '\n';
    }
    
    return 0;*/
}
