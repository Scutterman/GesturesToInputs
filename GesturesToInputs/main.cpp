#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "glfw/include/GLFW/glfw3.h"

#include "main.h"

#include <dinput.h>
#include <filesystem>
#include <iostream>
#include <thread>
#include <vector>

#include "Gesture.h"
#include "GesturesToInputsProcessor_CPU.h"
#include "MediaFoundationWebcam.h"
#include "Shader.h"

#include <opencv2/highgui.hpp>

using namespace GesturesToInputs;

std::vector<GestureInput> dragonGameGestures() {
    auto boostRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::CENTRE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto flySlowlyRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::FAR_LEFT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    auto upRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (unsigned int)VERTICAL_POSITION::TOP),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::TOP)
    };

    auto tiltLeftRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::LESS_THAN, "Green")
    };

    auto downRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (unsigned int)VERTICAL_POSITION::BOTTOM),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::BOTTOM)
    };

    auto tiltRightRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", GESTURE_RULE_COMPARISON_OPERATION::MORE_THAN, "Green")
    };

    return std::vector<GestureInput>{
        GestureInput(boostRules, DIK_F, "BOOST"),
        GestureInput(flySlowlyRules, DIK_SPACE, "SLOW DOWN"),
        GestureInput(upRules, DIK_W, "FLY UP"),
        GestureInput(tiltLeftRules, DIK_A, "TILT LEFT"),
        GestureInput(downRules, DIK_S, "FLY DOWN"),
        GestureInput(tiltRightRules, DIK_D, "TILT RIGHT"),
    };
}

std::vector<GestureInput> justCause2Gestures() {
    auto grappleRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto jumpRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto primaryWeaponRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::FAR_LEFT)
    };
    
    auto secondaryWeaponRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::LANDSCAPE),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    auto forwardRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (int)VERTICAL_POSITION::TOP)
    };

    auto leftRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::FAR_LEFT)
    };

    auto backwardRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", (unsigned int)VERTICAL_POSITION::BOTTOM),
    };

    auto rightRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Red", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Red", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto lookUpRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::TOP)
    };

    auto lookLeftRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::CENTRE)
    };

    auto lookDownRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", (unsigned int)VERTICAL_POSITION::BOTTOM),
    };

    auto lookRightRules = std::vector<GestureRule>{
        GestureRule(GESTURE_RULE_TYPE::ORIENTATION, "Green", (unsigned int)MARKER_ORIENTATION::PORTRAIT),
        GestureRule(GESTURE_RULE_TYPE::HORIZONTAL_COMPARE, "Green", (unsigned int)HORIZONTAL_POSITION::FAR_RIGHT)
    };

    return std::vector<GestureInput>{
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

std::vector<GestureInput> testGestures() {
    return std::vector<GestureInput>{
        GestureInput(std::vector<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Green", uint(VERTICAL_POSITION::BOTTOM)),
        }, DIK_G, "GREEN"),
        GestureInput(std::vector<GestureRule>{
            GestureRule(GESTURE_RULE_TYPE::VERTICAL_COMPARE, "Red", uint(VERTICAL_POSITION::BOTTOM)),
        }, DIK_R, "RED")
    };
}

GLFWwindow* window;

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
    GLuint boundingBox[4];
    GLuint objectTrackerIndex;
    GLuint padding[3];
};

struct TrackerData {
    float colour[4];
    uint area = 0;
    uint horizontalPosition = 0;
    uint verticalPosition = 0;
    uint orientation = 0;
    uint isVisibleInFrame = 0;
    uint padding[3];
    
    TrackerData(float _colour[4]) {
        colour[0] = _colour[0];
        colour[1] = _colour[1];
        colour[2] = _colour[2];
        colour[3] = _colour[3];
        padding[0] = padding[1] = padding[2] = 0;
    }
};

struct GestureRuleData {
    uint gestureIndex = 0;
    uint type = 0;
    int operation = 0;
    uint expectedValue = 0;
    uint compareTwoTrackers = 0;
    uint trackerIndex = 0;
    uint comparisonTrackerIndex = 0;
    uint padding = 0;
};

typedef cv::Vec<uint, 4> Vec4ui;

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
const int THRESHOLD_IMAGE_UNIT = 1;
const int OUTPUT_IMAGE_UNIT = 2;
const int SHADER_STORAGE_THRESHOLD = 3;
const int SHADER_STORAGE_OBJECT_SEARCH = 4;
const int RAW_DATA_IMAGE_UNIT = 5;
const int SHADER_STORAGE_TRACKERS = 6;
const int SHADER_STORAGE_GESTURE = 7;
const int SHADER_STORAGE_GESTURE_RULE = 8;

const unsigned int sampleColumns = 64;
const unsigned int sampleRows = 48;
const unsigned int totalSamples = sampleColumns * sampleRows;

bool opengl_has_errored = false;
GLuint vertex_array;
GLuint vertex_buffer, element_buffer;
GLint vpos_location, tex_location;

GLuint inputTextureHandle, thresholdTextureHandle, outputTextureHandle, rawDataTextureHandle;
const GLenum inputTextureUnit = GL_TEXTURE0, thresholdTextureUnit = GL_TEXTURE1, outputTextureUnit = GL_TEXTURE2, rawDataTextureUnit = GL_TEXTURE3;
int xMaxInstances, yMaxInstances, zMaxInstances, totalMaxInstances;

GLuint thresholdShaderBuffer, trackerShaderBuffer, computeShaderBuffer, gestureFoundShaderBuffer, gestureRuleShaderBuffer;

Shader hsvShader, thresholdShader, objectSearchShader, detectGesturesShader, displayShader, yuy2Shader;

cv::Mat source;
int sourceWidth, sourceHeight;

void error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int end(std::string message = "") {
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
    std::cout << message << std::endl;
    return 1;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    opengl_has_errored = true;
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}

int setup() {
    // TODO:: If no GPU or opengl version < 4.3 then fall back to cpu method.
    // TODO:: Cpu method may be optimisable since we only need rough bounding box. Moments can certainly be removed, and perhaps more optimisaitons.

    if (!glfwInit())
    {
        // Initialization failed
        return end("GLFW could not initialise OpenGL");
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
        return end("GLFW could not create a window");
    }
    
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return end("Could not set up GLAD");
    }

    glDebugMessageCallback(MessageCallback, NULL);
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

    return 0;
}

void bindImageData(GLenum textureUnit, unsigned char *image, int format) {
    glActiveTexture(textureUnit);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, sourceWidth, sourceHeight, format, GL_UNSIGNED_BYTE, image);
}

void bindImageHandle(GLuint* handle, GLenum textureUnit, int format = GL_RGBA32F) {
    glActiveTexture(textureUnit);
    glGenTextures(1, handle);
    glBindTexture(GL_TEXTURE_2D, GLuint(*handle));
    glTexStorage2D(GL_TEXTURE_2D, 1, format, sourceWidth, sourceHeight);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void bindInput() {
    bindImageHandle(&inputTextureHandle, inputTextureUnit);
    glBindImageTexture(INPUT_IMAGE_UNIT, inputTextureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

void convertYUY2ToRGB() {
    yuy2Shader.addShader(GL_COMPUTE_SHADER, "Convert_YUY2.comp");
    yuy2Shader.compile();
    yuy2Shader.use();

    auto inputTextureLocation = yuy2Shader.uniformLocation("inputImage");
    glUniform1i(inputTextureLocation, INPUT_IMAGE_UNIT);

    auto rawDataTextureLocation = yuy2Shader.uniformLocation("rawData");

    glUniform1i(rawDataTextureLocation, RAW_DATA_IMAGE_UNIT);
    
    bindImageHandle(&rawDataTextureHandle, rawDataTextureUnit, GL_RG8UI);
    glBindImageTexture(RAW_DATA_IMAGE_UNIT, rawDataTextureHandle, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RG8UI);
}

void convertToHSV() {
    hsvShader.addShader(GL_COMPUTE_SHADER, "ConvertToHSV.comp");
    hsvShader.compile();
    hsvShader.use();

    auto inputTextureLocation = hsvShader.uniformLocation("inputImage");

    glUniform1i(inputTextureLocation, INPUT_IMAGE_UNIT);

    auto thresholdTextureLocation = hsvShader.uniformLocation("thresholdTexture");
    bindImageHandle(&thresholdTextureHandle, thresholdTextureUnit);
    glBindImageTexture(THRESHOLD_IMAGE_UNIT, thresholdTextureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glUniform1i(thresholdTextureLocation, THRESHOLD_IMAGE_UNIT);

    auto outputImageTextureLocation = hsvShader.uniformLocation("outputImage");

    bindImageHandle(&outputTextureHandle, outputTextureUnit);
    glBindImageTexture(OUTPUT_IMAGE_UNIT, outputTextureHandle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glUniform1i(outputImageTextureLocation, OUTPUT_IMAGE_UNIT);

    auto isMirroredLocation = hsvShader.uniformLocation("inputIsMirrored");
    glUniform1ui(isMirroredLocation, 1);

    auto rgbCorrectionsLocation = hsvShader.uniformLocation("rgbCorrections");
    glUniform3fv(rgbCorrectionsLocation, 1, new float[3]{ 0.0f, 0.05f, 0.0f });

    auto lightenAmountLocation = hsvShader.uniformLocation("lightenAmount");
    glUniform1f(lightenAmountLocation, 0.5);
}

void threshold(std::vector<ThresholdData> items) {
    thresholdShader.addShader(GL_COMPUTE_SHADER, "Threshold.comp");
    thresholdShader.compile();
    thresholdShader.use();
    
    auto numberOfColoursLocation = thresholdShader.uniformLocation("numberOfColours");
    glUniform1ui(numberOfColoursLocation, items.size());

    auto thresholdTextureLocation = thresholdShader.uniformLocation("thresholdTexture");
    glUniform1i(thresholdTextureLocation, THRESHOLD_IMAGE_UNIT);

    glGenBuffers(1, &thresholdShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, thresholdShaderBuffer);

    uint thresholdDataSize = sizeof(ThresholdData) * items.size();
    ThresholdData* bufferItems;
    bufferItems = (ThresholdData*)malloc(thresholdDataSize);
    int i = 0;
    for (auto& item : items) { bufferItems[i] = item; i++; }
    glBufferData(GL_SHADER_STORAGE_BUFFER, thresholdDataSize, bufferItems, GL_STATIC_DRAW);
    free(bufferItems);
    
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_THRESHOLD, thresholdShaderBuffer);
}

void searchForObjects(std::vector<TrackerData> trackers) {
    objectSearchShader.addShader(GL_COMPUTE_SHADER, "ObjectBoundingBoxSearch_Pass1.comp");
    objectSearchShader.compile();
    objectSearchShader.use();

    auto objectTextureLocation = objectSearchShader.uniformLocation("thresholdTexture");
    glUniform1i(objectTextureLocation, THRESHOLD_IMAGE_UNIT);

    GLint samplePixelDimensions_location, sampleSize_location, passes_location, threshold_location;
    samplePixelDimensions_location = objectSearchShader.uniformLocation("samplePixelDimensions");
    sampleSize_location = objectSearchShader.uniformLocation("sampleSize");
    passes_location = objectSearchShader.uniformLocation("numberOfPasses");
    threshold_location = objectSearchShader.uniformLocation("threshold");

    int samplePixelDimensions[] = { sourceWidth / sampleColumns, sourceHeight / sampleRows };
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
    glUniform1i(outputImageTextureLocation, OUTPUT_IMAGE_UNIT);

    uint objectDataSize = sizeof(ObjectSearchData) * totalSamples;

    glGenBuffers(1, &computeShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, computeShaderBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, objectDataSize, NULL, GL_STATIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_OBJECT_SEARCH, computeShaderBuffer);


    glGenBuffers(1, &trackerShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, trackerShaderBuffer);

    uint trackerDataSize = sizeof(TrackerData) * trackers.size();
    auto bufferItems = (TrackerData*)malloc(trackerDataSize);
    int i = 0;
    for (auto& tracker : trackers) { bufferItems[i] = tracker; i++; }
    glBufferData(GL_SHADER_STORAGE_BUFFER, trackerDataSize, bufferItems, GL_STATIC_DRAW);
    free(bufferItems);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_TRACKERS, trackerShaderBuffer);
}

unsigned int* detectGesturesSetup(unsigned int numberOfGestures, std::vector<GestureRuleData>* rules) {
    detectGesturesShader.addShader(GL_COMPUTE_SHADER, "DetectGestures.comp");
    detectGesturesShader.compile();
    detectGesturesShader.use();

    glGenBuffers(1, &gestureFoundShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gestureFoundShaderBuffer);
    auto flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT;
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, numberOfGestures * sizeof(unsigned int), NULL, flags);
    unsigned int* gestureFoundDataPointer;
    gestureFoundDataPointer = (unsigned int*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, numberOfGestures * sizeof(unsigned int), flags);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_GESTURE, gestureFoundShaderBuffer);


    glGenBuffers(1, &gestureRuleShaderBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gestureRuleShaderBuffer);

    uint rulesDataSize = sizeof(GestureRuleData) * rules->size();
    auto bufferItems = (GestureRuleData*)malloc(rulesDataSize);
    int i = 0;
    for (auto& rule : *rules) { bufferItems[i] = rule; i++; }
    glBufferData(GL_SHADER_STORAGE_BUFFER, rulesDataSize, bufferItems, GL_STATIC_DRAW);
    free(bufferItems);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_GESTURE_RULE, gestureRuleShaderBuffer);


    glBindBuffer(GL_SHADER_STORAGE_BUFFER, trackerShaderBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, SHADER_STORAGE_TRACKERS, trackerShaderBuffer);
    return gestureFoundDataPointer;
}

void displayOutputSetup() {
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    bool vertexShaderSuccess, fragmentShaderSuccess, shaderCompileSuccess;

    vertexShaderSuccess = displayShader.addShader(GL_VERTEX_SHADER, "test.vert");

    fragmentShaderSuccess = displayShader.addShader(GL_FRAGMENT_SHADER, "test.frag");

    shaderCompileSuccess = displayShader.compile();

    displayShader.use();

    vpos_location = displayShader.attributeLocation("aPosition");

    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);

    tex_location = displayShader.attributeLocation("aTexCoord");

    glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(2 * sizeof(float)));

    auto outputImageTextureLocation = displayShader.uniformLocation("outputImage");
    glUniform1i(outputImageTextureLocation, OUTPUT_IMAGE_UNIT);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    glViewport(0, 0, width, height);
    }

void displayOutput() {
    displayShader.use();
    glEnableVertexAttribArray(vpos_location);
    glEnableVertexAttribArray(tex_location);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(vpos_location);
    glDisableVertexAttribArray(tex_location);
    glfwSwapBuffers(window);
}

void debugDisplayTexture(GLenum textureUnit, std::string windowName) {
    glActiveTexture(textureUnit);
    unsigned char* gl_texture_bytes = (unsigned char*)malloc(sizeof(unsigned char) * sourceWidth * sourceHeight * 3);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, gl_texture_bytes);
    cv::imshow(windowName, cv::Mat(sourceHeight, sourceWidth, CV_8UC3, gl_texture_bytes));
    free(gl_texture_bytes);
}

const double cMultiplier = 1.164383;
const double erMultiplier = 1.596027;
const double dgMultiplier = 0.391762;
const double egMultiplier = 0.812968;
const double dbMultiplier = 2.017232;

int clip(int in) {
    return in < 0 ? 0 : in > 255 ? 255 : in;
}

void debugYUY2Bytes(unsigned char* gl_texture_bytes, std::string windowName) {
    cv::Mat cvMat = cv::Mat::zeros(cv::Size(sourceWidth, sourceHeight), CV_8UC3);
    auto p = cvMat.data;
    unsigned int destinationIndex = 0;

    // YUY2 to RGB conversion from information at [https://docs.microsoft.com/en-us/previous-versions/aa904813(v=vs.80)]
    for (unsigned int yCoord = 0; yCoord < sourceHeight; yCoord++) {
        for (unsigned int xCoord = 0; xCoord < sourceWidth; xCoord += 2) {
            unsigned int sourceIndex = (xCoord + (yCoord * sourceWidth)) * 2;
            unsigned char y1 = gl_texture_bytes[sourceIndex], u = gl_texture_bytes[sourceIndex + 1], y2 = gl_texture_bytes[sourceIndex + 2], v = gl_texture_bytes[sourceIndex + 3];
            int c = y1 - 16, d = u - 128, e = v - 128, f = y2 - 16;

            int r1 = clip(round((c * cMultiplier) + (e * erMultiplier)));
            int g1 = clip(round((c * cMultiplier) - (d * dgMultiplier) - (e * egMultiplier)));
            int b1 = clip(round((c * cMultiplier) + d * dbMultiplier));
            int r2 = clip(round((f * cMultiplier) + (e * erMultiplier)));
            int g2 = clip(round((f * cMultiplier) - (d * dgMultiplier) - (e * egMultiplier)));
            int b2 = clip(round((f * cMultiplier) + d * dbMultiplier));

            // BGR
            p[destinationIndex + 0] = b1;
            p[destinationIndex + 1] = g1;
            p[destinationIndex + 2] = r1;
            p[destinationIndex + 3] = b2;
            p[destinationIndex + 4] = g2;
            p[destinationIndex + 5] = r2;
            destinationIndex += 6;
        }
    }

    cv::imshow(windowName, cvMat);
}

void debugYUY2Texture() {
    glActiveTexture(rawDataTextureUnit);
    unsigned char* gl_texture_bytes = (unsigned char*)malloc(sizeof(unsigned char) * sourceWidth * sourceHeight * 2);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, gl_texture_bytes);
    debugYUY2Bytes(gl_texture_bytes, "raw image");
    free(gl_texture_bytes);
}

void getRulesFromGestures(std::vector<GestureInput>* gestures, std::map<std::string, unsigned int>* trackerNameIndexMap, std::vector<GestureRuleData>* out) {
    unsigned int gestureIndex = 0;
    for (auto& gesture : *gestures) {
        for (auto& rule : gesture.getRules()) {
            GestureRuleData ruleData;
            ruleData.gestureIndex = gestureIndex;
            ruleData.type = unsigned int(rule.getType());
            ruleData.operation = int(rule.getOperation());
            ruleData.expectedValue = rule.getExpectedValue();
            ruleData.compareTwoTrackers = rule.isComparingTwoTrackers() ? 1 : 0;
            ruleData.trackerIndex = trackerNameIndexMap->at(rule.trackerName);
            ruleData.comparisonTrackerIndex = rule.isComparingTwoTrackers() ? trackerNameIndexMap->at(rule.comparisonTrackerName) : 0;
            out->push_back(ruleData);
        }
        gestureIndex++;
    }
}

int main(int argc, char** argv)
{
    bool useGPU = true;
    Shader::setRoot(std::filesystem::path(argv[0]).parent_path() / "shaders");

    if (useGPU) {
        MediaFoundationWebcam* webcam = new MediaFoundationWebcam();
        PerformanceTimer timer;
        timer.Start();
        std::thread t1(&MediaFoundationWebcam::CreateVideoCaptureDevice, webcam);
        t1.detach();

        webcam->wait();
        sourceWidth = webcam->getWidth(); sourceHeight = webcam->getHeight();

        int status = setup();
        if (status != 0) {
            return status;
        }

        std::vector<ThresholdData> thresholdData;
        float low[4] = { 80, 111, 110, 255 };
        float high[4] = { 95, 255, 255, 255 };
        float tracker[4] = { 87, 183, 183, 255 };
        thresholdData.push_back(ThresholdData(low, high, tracker));

        float redtracker[4] = { 174, 179, 205, 255 };
        float red1low[4] = { 167, 159, 45, 255 };
        float red1high[4] = { 179, 240, 246, 255 };
        float red2low[4] = { 0, 104, 151, 255 };
        float red2high[4] = { 10, 255, 255, 255 };
        thresholdData.push_back(ThresholdData(red1low, red1high, redtracker));
        thresholdData.push_back(ThresholdData(red2low, red2high, redtracker));

        std::vector<TrackerData> trackers = { TrackerData(tracker), TrackerData(redtracker) };

        auto gestures = testGestures();
        unsigned int gestureCount = gestures.size();
        std::vector<unsigned int> gestureData(gestureCount);
        std::map<std::string, unsigned int> namesToIndex = { {"Green", 0}, {"Red", 1} };
        auto rules = new std::vector<GestureRuleData>;
        getRulesFromGestures(&gestures, &namesToIndex, rules);

        PerformanceTimer perf;
        bindInput();
        convertYUY2ToRGB();
        convertToHSV();
        threshold(thresholdData);
        searchForObjects(trackers);
        unsigned int* gestureFoundDataPointer = detectGesturesSetup(gestureCount, rules);
        displayOutputSetup();

        auto length = webcam->getWidth() * webcam->getHeight() * webcam->getBytesPerPixel();
        while (!glfwWindowShouldClose(window) && !opengl_has_errored)
        {
            webcam->wait();

            perf.Start();
            auto bytes = webcam->getData();

            yuy2Shader.use();
            bindImageData(rawDataTextureUnit, bytes, GL_RG_INTEGER);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glDispatchCompute(sourceWidth / 2, sourceHeight, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            hsvShader.use();
            // TODO:: Allow more formats. If YUY2 it can be converted, if it's a format GLTexSubImage2D knows about it can be uploaded directly
            //bindImageData(inputTextureUnit, source.ptr(), GL_BGR);
            //glMemoryBarrier(GL_ALL_BARRIER_BITS);
            glDispatchCompute(sourceWidth, sourceHeight, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            thresholdShader.use();
            glDispatchCompute(sourceWidth, sourceHeight, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            //debugDisplayTexture(thresholdTextureUnit, "threshold");

            objectSearchShader.use();
            glDispatchCompute(sampleColumns, sampleRows, trackers.size());
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            detectGesturesShader.use();
            glDispatchCompute(rules->size(), 1, 1);
            glMemoryBarrier(GL_ALL_BARRIER_BITS);

            memcpy(gestureData.data(), gestureFoundDataPointer, gestureCount * sizeof(unsigned int));
            std::cout << "Processed Frame: ";  perf.End();

            perf.Start();
            auto gesture = Gesture::getInstance();
            gesture->reset();
            unsigned int i = 0;
            for (auto& g : gestureData) {
                gesture->handleInput(&gestures.at(i), g == 1);
                if (g == 1) {
                    std::cout << gestures.at(i).getDebugMessage() << std::endl;
                }
                i++;
            }
            gesture->complete();

            std::cout << "Processed Gestures: ";  perf.End(); std::cout << std::endl;

            perf.Start();
            displayOutput();
            std::cout << "Output displayed in "; perf.End();
            std::cout << std::endl << std::endl << std::endl;

            glfwPollEvents();
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, gestureFoundShaderBuffer);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        webcam->Close();
        std::cout << webcam->framesCollected << " frames collected in "; timer.End();
        webcam->Release();

        return end();
    }
    else {
        try {
            auto redTrackerValues = std::vector<TrackerValues> { TrackerValues(0, 10, 145, 215, 90, 240), TrackerValues(165, 179, 0, 255, 0, 255) };
            auto greenTrackerValues = std::vector<TrackerValues> { TrackerValues(80, 95, 111, 255, 90, 255) };
        
            auto redTracker = new Tracker("Red", redTrackerValues, cv::Scalar(0, 0, 255));
            auto greenTracker = new Tracker("Green", greenTrackerValues, cv::Scalar(0, 255, 0));
            std::map<std::string, Tracker*> trackers = {
                { "Red",  redTracker },
                { "Green", greenTracker }
            };

            auto gestures = justCause2Gestures();

            auto processor = GesturesToInputsProcessor_CPU(trackers, &gestures);
            processor.webcamIndex = 0;
            processor.webcamMirrored = true;
            processor.run();
        }
        catch (int e) {
            std::cout << "An exception occurred. Exception Nr. " << e << '\n';
        }
    
        return 0;
    }
}
