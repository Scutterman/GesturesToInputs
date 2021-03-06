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
    unsigned int computeX = 1, computeY = 1, computeZ = 1;
public:
    static void setRoot(std::filesystem::path rootPath);
    void compileCompute(std::string fileName, unsigned int x, unsigned int y, unsigned int z);
    GLuint getHandle();
    bool addShader(int shaderType, std::string pathName);
    bool compile();
    void use();
    int attributeLocation(const GLchar* name);
    int uniformLocation(const GLchar* name);

    void setUniform(const GLchar* name, int value);
    void setUniform(const GLchar* name, unsigned int value);
    void setUniform(const GLchar* name, float value);
    void setUniform2(const GLchar* name, unsigned int value[2]);
    void setUniform3(const GLchar* name, float value[3]);

    void compute();
};
