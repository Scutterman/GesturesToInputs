#pragma once
#include <vector>
#include "glad/glad.h"
#include <string>
#include <filesystem>

class Shader
{
private:
    std::vector<GLuint> shaderHandles;
    GLuint programHandle;
    std::string getShaderContents(std::string shaderPath);
    static std::filesystem::path root;
public:
    static void setRoot(std::filesystem::path rootPath);
    GLuint getHandle();
    bool addShader(int shaderType, std::string pathName);
    bool compile();
    void use();
    int attributeLocation(const GLchar* name);
    int uniformLocation(const GLchar* name);
};
