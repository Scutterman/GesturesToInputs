#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "glfw/include/GLFW/glfw3.h"

#include "main.h"

#include <iostream>

#include "GesturesToInputs.h"

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
    return 0;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

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

static const char* vertex_shader_text =
"#version 330 core\n"
"layout (location = 0) in vec2 aPosition;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 texCoord;\n"
"void main()\n"
"{\n"
"    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
"    texCoord = aTexCoord;\n"
"}\n";

static const char* fragment_shader_text =
"#version 330\n"
"out vec4 FragColor;\n"
"in vec2 texCoord;\n"
"uniform sampler2D texture0;\n"
"uniform int texture0Height;\n"
"vec3 rgb2hsv(vec3 rgb)\n"
"{\n"
"   vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);\n"
"   vec4 p = rgb.g < rgb.b ? vec4(rgb.bg, K.wz) : vec4(rgb.gb, K.xy);\n"
"   vec4 q = rgb.r < p.x ? vec4(p.xyw, rgb.r) : vec4(rgb.r, p.yzx);\n"
"   float d = q.x - min(q.w, q.y);\n"
"   float e = 1.0e-10;\n"
"   return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);\n"
"}\n"
"vec3 hsv2rgb(vec3 hsv)\n"
"{\n"
"   vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
"   vec3 p = abs(fract(hsv.xxx + K.xyz) * 6.0 - K.www);\n"
"   return vec3(hsv.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), hsv.y));\n"
"}\n"
"void main()\n"
"{\n"
"   vec4 bgr = texture(texture0, vec2(texCoord.x, texture0Height - texCoord.y));"
"   vec3 rgb = bgr.zyx;\n"
"   vec3 hsv = rgb2hsv(rgb).xyz;\n"
"   FragColor = vec4(hsv2rgb(hsv).xyz, 1.0);\n"
"}\n";

bool opengl_has_errored = false;
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

int main(int argc, char** argv)
{
    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
    {
        // Initialization failed
        return end("GLFW could not initialise OpenGL");
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    auto cam = Webcam();
    auto frame = cam.next();
    
    window = glfwCreateWindow(frame.source.size().width, frame.source.size().height, "Output", NULL, NULL);
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

    GLuint vertex_array, vertex_buffer, element_buffer, vertex_shader, fragment_shader, program;
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

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
    checkError("vertex shader");
    
    GLint success;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(vertex_shader, sizeof(InfoLog), NULL, InfoLog);
        std::cout << "Error compiling vertex shader" << InfoLog << '\n';
    }
    
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
    checkError("fragment shader");
    
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(fragment_shader, sizeof(InfoLog), NULL, InfoLog);
        std::cout << "Error compiling fragment shader" << InfoLog << '\n';
    }

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0) {
        GLchar InfoLog[1024];
        glGetProgramInfoLog(program, sizeof(InfoLog), NULL, InfoLog);
        std::cout << "Error linking shader program: " << InfoLog << '\n';
    }
    checkError("program");

    glUseProgram(program);
    checkError("use program 1");
    
    vpos_location = glGetAttribLocation(program, "aPosition");
    checkError("get vertex location");
    
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)0);
    glEnableVertexAttribArray(vpos_location);
    checkError("vertex attribute");
    
    tex_location = glGetAttribLocation(program, "aTexCoord");
    checkError("get texture location");

    glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(tex_location);
    checkError("texture attribute");

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    checkError("clear colour");
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    checkError("texture options");
    
    GLuint textureHandle;
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    checkError("generating textures");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.source.cols, frame.source.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.source.ptr());
    checkError("sending data");
    glGenerateMipmap(GL_TEXTURE_2D);
    checkError("textures bound");

    texture0Height_location = glGetAttribLocation(program, "texture0Height");
    checkError("get height location");
    glUniform1i(texture0Height_location, frame.source.rows);

    while (!glfwWindowShouldClose(window) && !opengl_has_errored)
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        checkError("viewport size");
        glClear(GL_COLOR_BUFFER_BIT);
        checkError("clear");

        glUseProgram(program);
        checkError("use program");
        glBindTexture(GL_TEXTURE_2D, textureHandle);
        checkError("bind texture");
        glBindVertexArray(vertex_array);
        checkError("bind array");
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
        checkError("draw");

        glfwSwapBuffers(window);
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
