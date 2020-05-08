#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <glad/glad.h>
#include "file.hpp"
#include "output.hpp"

class Shader {
public:
  Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
  ~Shader();
  GLint GetUniformLocation(const char* name);
  
  GLuint program;
  GLint modelLoc;
  GLint viewLoc;
  GLint projectionLoc;
};

#endif
