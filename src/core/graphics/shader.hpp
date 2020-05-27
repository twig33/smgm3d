#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <glad/glad.h>

GLuint CreateShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
GLint GetUniformLocation(GLuint program, const char* name);

#endif
