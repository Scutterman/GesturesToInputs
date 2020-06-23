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

class DetectedObjects {
public:
    std::vector<cv::Vec4i> boundingBoxes;
    cv::Scalar colour;
};

std::vector<DetectedObjects> trackerObjects;

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

bool doErrorCheck = false;
const unsigned int indices[] = { 0, 1, 2, 3, 0, 2 };
const int INPUT_IMAGE_UNIT = 0;
const int THRESHOLD_IMAGE_UNIT = 1;
const int OUTPUT_IMAGE_UNIT = 2;

const int SHADER_STORAGE_THRESHOLD = 3;
const int SHADER_STORAGE_OBJECT_SEARCH = 4;

const unsigned int sampleColumns = 64;
const unsigned int sampleRows = 48;
const unsigned int totalSamples = sampleColumns * sampleRows;

bool opengl_has_errored = false;
GLuint vertex_array;
GLuint inputTextureHandle, thresholdTextureHandle, outputTextureHandle;
const GLenum inputTextureUnit = GL_TEXTURE0, thresholdTextureUnit = GL_TEXTURE1, outputTextureUnit = GL_TEXTURE2;
int xMaxInstances, yMaxInstances, zMaxInstances, totalMaxInstances;

GLuint thresholdShaderBuffer, computeShaderBuffer;

std::filesystem::path basePath;
Shader hsvShader, thresholdShader, objectSearchShader, displayShader;
int trackerColourLocation;

cv::Mat source;
int sourceWidth, sourceHeight;

void checkError(std::string stage) {
    if (!doErrorCheck) { return; }
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

int setup() {
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

    window = glfwCreateWindow(sourceWidth, sourceHeight, "Output", NULL, NULL);
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

    int work_group_size[3];
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_group_size[0]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_group_size[1]);
    glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_group_size[2]);
    checkError("group size");
    xMaxInstances = work_group_size[0];
    yMaxInstances = work_group_size[1];
    zMaxInstances = work_group_size[2];
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &totalMaxInstances);
    checkError("total size");
    
    std::cout << std::endl << "(" << xMaxInstances << ", " << yMaxInstances << ", " << zMaxInstances << ") = " << totalMaxInstances << std::endl;

    return 0;
}

void bindOpenCVImage(GLenum textureUnit, cv::Mat image) {
    glActiveTexture(textureUnit);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sourceWidth, sourceHeight, GL_BGR, GL_UNSIGNED_BYTE, image.ptr());
    checkError("sending data");
}

void bindImageHandle(GLuint* handle, GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glGenTextures(1, handle);
    glBindTexture(GL_TEXTURE_2D, GLuint(*handle));
    checkError("generating textures");
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, sourceWidth, sourceHeight);
    checkError("texture storage");

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkError("texture options");
}

void debugBoundingBoxes(DetectedObjects* objects) {
    auto objectBufferData = (ObjectSearchData*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY);

    int samplePixelColumns = sourceWidth / sampleColumns;
    int samplePixelRows = sourceHeight / sampleRows;
    for (int i = 0; i < totalSamples; i++) {
        if (objectBufferData[i].isObjectTopLeft == 1) {
            auto x = objectBufferData[i].boundingBox[0] * samplePixelColumns;
            auto y = objectBufferData[i].boundingBox[1] * samplePixelRows;
            auto width = (objectBufferData[i].boundingBox[2] * samplePixelColumns) - x;
            auto height = (objectBufferData[i].boundingBox[3] * samplePixelRows) - y;
            objects->boundingBoxes.push_back({ x, y, width, height });
        }
    }
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

void convertToHSV() {
    hsvShader.addShader(GL_COMPUTE_SHADER, (basePath / "shaders" / "ConvertToHSV.comp").string());
    checkError("hsv shader");
    hsvShader.compile();
    checkError("hsv shader compile");
    hsvShader.use();

    auto inputTextureLocation = hsvShader.uniformLocation("inputImage");
    checkError("Get Input Texture Location");
    
    bindImageHandle(&inputTextureHandle, inputTextureUnit);
    glBindImageTexture(INPUT_IMAGE_UNIT, inputTextureHandle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    checkError("Bind input image");
    glUniform1i(inputTextureLocation, INPUT_IMAGE_UNIT);
    checkError("Set Texture Location");

    auto thresholdTextureLocation = hsvShader.uniformLocation("thresholdTexture");
    checkError("Get Threshold Texture Location");
    
    bindImageHandle(&thresholdTextureHandle, thresholdTextureUnit);
    glBindImageTexture(THRESHOLD_IMAGE_UNIT, thresholdTextureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    checkError("Bind Threshold Image");
    glUniform1i(thresholdTextureLocation, THRESHOLD_IMAGE_UNIT);
    checkError("Set Texture Location");

    auto outputImageTextureLocation = hsvShader.uniformLocation("outputImage");
    checkError("Get Output Image Location");

    bindImageHandle(&outputTextureHandle, outputTextureUnit);
    glBindImageTexture(OUTPUT_IMAGE_UNIT, outputTextureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    checkError("Bind output image");
    glUniform1i(outputImageTextureLocation, OUTPUT_IMAGE_UNIT);
    checkError("Set Texture Location");

    auto isMirroredLocation = hsvShader.uniformLocation("inputIsMirrored");
    checkError("Get isMirrored Location");
    glUniform1ui(isMirroredLocation, 1);
    checkError("Set isMirrored");
}

void threshold(std::vector<ThresholdData> items) {
    thresholdShader.addShader(GL_COMPUTE_SHADER, (basePath / "shaders" / "Threshold.comp").string());
    checkError("threshold shader");
    thresholdShader.compile();
    checkError("threshold shader compile");
    thresholdShader.use();
    
    auto numberOfColoursLocation = thresholdShader.uniformLocation("numberOfColours");
    checkError("Get Threshold number of colours Location");
    glUniform1ui(numberOfColoursLocation, items.size());
    checkError("Set threshold number of colours Location");

    auto thresholdTextureLocation = thresholdShader.uniformLocation("thresholdTexture");
    checkError("Get Threshold Texture Location");
    glUniform1i(thresholdTextureLocation, THRESHOLD_IMAGE_UNIT);
    checkError("Set Texture Location");

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
}

void searchForObjects() {
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
    samplePixelDimensions[0] = sourceWidth / sampleColumns;
    samplePixelDimensions[1] = sourceHeight / sampleRows;
    glUniform2iv(samplePixelDimensions_location, 1, samplePixelDimensions);

    int sampleSize[2];
    sampleSize[0] = sampleColumns;
    sampleSize[1] = sampleRows;
    glUniform2iv(sampleSize_location, 1, sampleSize);

    unsigned int numberOfPasses = 100;
    glUniform1ui(passes_location, numberOfPasses);

    unsigned int thresholdArea = 500;
    glUniform1ui(threshold_location, thresholdArea);

    auto outputImageTextureLocation = objectSearchShader.uniformLocation("outputImage");
    checkError("Get Output Image Location");
    glUniform1i(outputImageTextureLocation, OUTPUT_IMAGE_UNIT);
    checkError("Set Texture Location");

    uint objectDataSize = sizeof(ObjectSearchData) * totalSamples;

    glGenBuffers(1, &computeShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, objectDataSize, NULL, GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_OBJECT_SEARCH, computeShaderBuffer);

    checkError("After Buffer");

    trackerColourLocation = objectSearchShader.uniformLocation("trackerColour");
    checkError("Get Tracker Colour Location");
}

void displayOutputSetup() {
    GLuint vertex_buffer, element_buffer;
    GLint vpos_location, tex_location, texture0Height_location;
    
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

    bool vertexShaderSuccess, fragmentShaderSuccess, shaderCompileSuccess;
    std::filesystem::path vertexPath = basePath / "shaders" / "test.vert", fragmentPath = basePath / "shaders" / "test.frag";

    vertexShaderSuccess = displayShader.addShader(GL_VERTEX_SHADER, vertexPath.string());
    checkError("vertex shader");

    fragmentShaderSuccess = displayShader.addShader(GL_FRAGMENT_SHADER, fragmentPath.string());
    checkError("fragment shader");

    shaderCompileSuccess = displayShader.compile();
    checkError("program");

    displayShader.use();
    checkError("use program 1");

    vpos_location = displayShader.attributeLocation("aPosition");
    checkError("get vertex location");

    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vpos_location);
    checkError("vertex attribute");

    tex_location = displayShader.attributeLocation("aTexCoord");
    checkError("get texture location");

    glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(tex_location);
    checkError("texture attribute");

    auto outputImageTextureLocation = displayShader.uniformLocation("outputImage");
    checkError("Get Output Image Location");
    glUniform1i(outputImageTextureLocation, OUTPUT_IMAGE_UNIT);
    checkError("Set Output Image Location");

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    checkError("viewport size");
}

void displayOutput() {
    displayShader.use();
    checkError("use program");
    glBindVertexArray(vertex_array);
    checkError("bind array");
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    checkError("draw");

    glfwSwapBuffers(window);
}

void debugDisplayTexture(GLenum textureUnit, std::string windowName) {
    glActiveTexture(textureUnit);
    checkError(windowName + " display texture bind");
    unsigned char* gl_texture_bytes = (unsigned char*)malloc(sizeof(unsigned char) * sourceWidth * sourceHeight * 3);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, gl_texture_bytes);
    checkError(windowName + " display texture get");
    cv::imshow(windowName, cv::Mat(sourceHeight, sourceWidth, CV_8UC3, gl_texture_bytes));
    free(gl_texture_bytes);
}

int main(int argc, char** argv)
{
    // doErrorCheck = true;
    basePath = std::filesystem::path(argv[0]).parent_path();
    auto cam = Webcam();
    source = cam.next().source.clone();
    sourceWidth = source.cols;
    sourceHeight = source.rows;
    
    int status = setup();
    if (status != 0) {
        return status;
    }

    std::vector<ThresholdData> trackers;
    float low[4] = { 80, 111, 110, 255 };
    float high[4] = { 95, 255, 255, 255 };
    float tracker[4] = { 87, 183, 183, 255 };
    trackers.push_back(ThresholdData(low, high, tracker));

    float redtracker[4] = { 174, 179, 205, 255 };
    float red1low[4] = { 169, 104, 151, 255 };
    float red1high[4] = { 179, 255, 255, 255 };
    float red2low[4] = { 0, 104, 151, 255 };
    float red2high[4] = { 10, 255, 255, 255 };
    trackers.push_back(ThresholdData(red1low, red1high, redtracker));
    trackers.push_back(ThresholdData(red2low, red2high, redtracker));

    auto trackerColours = std::vector<cv::Scalar> {
        cv::Scalar(tracker[0], tracker[1], tracker[2], tracker[3]),
        cv::Scalar(redtracker[0], redtracker[1], redtracker[2], redtracker[3])
    };
    
    PerformanceTimer perf;
    convertToHSV();
    threshold(trackers);
    searchForObjects();
    displayOutputSetup();
    
    while (!glfwWindowShouldClose(window) && !opengl_has_errored)
    {
        perf.Start();
        //trackerObjects.clear();
        source = cam.next().source.clone();
        std::cout << "source capture in "; perf.End();
        
        perf.Start();
        hsvShader.use();
        bindOpenCVImage(inputTextureUnit, source);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        checkError("Bind source image");
        glDispatchCompute(sourceWidth, sourceHeight, 1);
        checkError("After Shader");
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        checkError("After Barrier");

        thresholdShader.use();
        glDispatchCompute(sourceWidth, sourceHeight, 1);
        checkError("After Shader");
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        checkError("After Barrier");

        objectSearchShader.use();
        for (auto& colour : trackerColours) {
            float uniformColour[4] = { colour[0], colour[1], colour[2], colour[3] };
            glUniform4fv(trackerColourLocation, 1, uniformColour);
            checkError("Set Tracker Colour Location");
            glDispatchCompute(sampleColumns, sampleRows, 1);
            checkError("After Shader");
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            checkError("After Barrier");
            /*
            DetectedObjects objects;
            objects.colour = colour;
            debugBoundingBoxes(&objects);
            trackerObjects.push_back(objects);
            */
        }
        std::cout << "everything else in "; perf.End();

        perf.Start();
        displayOutput();
        std::cout << "output displayed in "; perf.End();
        std::cout << std::endl << std::endl << std::endl;

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
