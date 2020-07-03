#pragma once
#include <vector>
#include "glad/glad.h"
#include <string>

class Shader
{
private:
    std::vector<GLuint> shaderHandles;
    GLuint programHandle;
    std::string getShaderContents(std::string shaderPath);
public:
    GLuint getHandle();
    bool addShader(int shaderType, std::string pathName);
    bool compile();
    void use();
    int attributeLocation(const GLchar* name);
    int uniformLocation(const GLchar* name);
};
