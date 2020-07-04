#pragma once
#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "glfw/include/GLFW/glfw3.h"
#include "structures.h"
#include "GesturesToInputsProcessor.h"
#include "MediaFoundationWebcam.h"
#include "Shader.h"

namespace GesturesToInputs {
    class GesturesToInputsPorcessor_OpenGL : GesturesToInputsProcessor
    {
    private:
        const struct
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

        const unsigned int indices[6] = { 0, 1, 2, 3, 0, 2 };
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
    
    private:
        GLFWwindow* window;
        MediaFoundationWebcam* webcam;
        unsigned int sourceWidth, sourceHeight;
        unsigned int* gestureFoundDataPointer;
        GLuint vertex_array;
        GLuint vertex_buffer, element_buffer;
        GLint vpos_location, tex_location;

        GLuint inputTextureHandle, thresholdTextureHandle, outputTextureHandle, rawDataTextureHandle;
        const GLenum inputTextureUnit = GL_TEXTURE0, thresholdTextureUnit = GL_TEXTURE1, outputTextureUnit = GL_TEXTURE2, rawDataTextureUnit = GL_TEXTURE3;
        int xMaxInstances, yMaxInstances, zMaxInstances, totalMaxInstances;

        GLuint thresholdShaderBuffer, trackerShaderBuffer, computeShaderBuffer, gestureFoundShaderBuffer, gestureRuleShaderBuffer;
        Shader hsvShader, thresholdShader, objectSearchShader, detectGesturesShader, displayShader, yuy2Shader;
        
    public:
        bool opengl_has_errored = false;
        GesturesToInputsPorcessor_OpenGL(
            MediaFoundationWebcam* webcam,
            std::vector<ThresholdData>* thresholdData,
            std::vector<TrackerData>* trackers,
            std::vector<GestureRuleData>* rules,
            std::vector<GestureInput>* gestures
        );
        ~GesturesToInputsPorcessor_OpenGL();

        void run();
        void bindImageHandle(GLuint* handle, GLenum textureUnit, int binding, int format = GL_RGBA32F, int access = GL_READ_WRITE);
        void convertYUY2ToRGB();
        void convertToHSV();
        void threshold(std::vector<ThresholdData>* items);
        void searchForObjects(std::vector<TrackerData>* trackers);
        void detectGesturesSetup(unsigned int numberOfGestures, std::vector<GestureRuleData>* rules);
        void displayOutputSetup();
        void displayOutput();
        void debugDisplayTexture(GLenum textureUnit, std::string windowName);
        void createEmptyStorageBuffer(GLuint* bufferHandle, unsigned int dataSize, int binding);
        unsigned int* createEmptyPersistantStorageBuffer(GLuint* bufferHandle, unsigned int dataSize, int binding);
        template<typename T>
        void createStorageBuffer(GLuint* bufferHandle, std::vector<T>* items, int binding);
    };
}
