#include "Shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::string Shader::getShaderContents(std::string shaderPath)
{
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    shaderFile.open(shaderPath);
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}

bool Shader::addShader(int shaderType, std::string shaderPath)
{
    GLuint shaderHandle;
    shaderHandle = glCreateShader(shaderType);
    auto contents = getShaderContents(shaderPath);
    auto contentsPointer = contents.c_str();
    glShaderSource(shaderHandle, 1, &contentsPointer, NULL);
    glCompileShader(shaderHandle);
    
    GLint success;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info[512];
        glGetShaderInfoLog(shaderHandle, 512, NULL, info);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info << std::endl;
        return false;
    };

    shaderHandles.push_back(shaderHandle);
    return true;
}

bool Shader::compile()
{
    programHandle = glCreateProgram();
    for (auto& shaderHandle : shaderHandles) { glAttachShader(programHandle, shaderHandle); }
    glLinkProgram(programHandle);

    GLint success;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &success);
    if (!success) {
        char info[512];
        glGetProgramInfoLog(programHandle, 512, NULL, info);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info << std::endl;
        return false;
    }

    for (auto& shaderHandle : shaderHandles) { glDeleteShader(shaderHandle); }
    return true;
}

void Shader::use() {
    glUseProgram(programHandle);
}

int Shader::attributeLocation(const GLchar* name)
{
    return glGetAttribLocation(programHandle, name);
}

int Shader::uniformLocation(const GLchar* name)
{
    return glGetUniformLocation(programHandle, name);
}
