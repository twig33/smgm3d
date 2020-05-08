#include <stdexcept>
#include <glad/glad.h>
#include "shader.hpp"
#include "file.hpp"
#include "output.hpp"

GLint GetUniformLocation(GLuint program, const char* name){
  int loc = glGetUniformLocation(program, name);
  if (loc == -1){
    Output::stream << "Shader: Couldn't get location of uniform \"" << name << "\"\n";
    return -1;
  }
  return loc;
}

GLuint CreateShaderProgram (const char* vertexShaderPath, const char* fragmentShaderPath){
  GLuint program;
  
  if (!FileExists(vertexShaderPath)){
    Output::stream << "Vertex shader source file not found\n";
    return 0;
  }
  if (!FileExists(fragmentShaderPath)){
    Output::stream << "Fragment shader source file not found\n";
    return 0;
  }
      
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  unsigned long size;
      
  ReadFileSize(vertexShaderPath, &size);
  char* vertexShaderSrc = (char*)calloc(size, sizeof(char));
  ReadFileCStr(vertexShaderPath, vertexShaderSrc, size);

  ReadFileSize(fragmentShaderPath, &size);
  char* fragmentShaderSrc = (char*)calloc(size, sizeof(char));
  ReadFileCStr(fragmentShaderPath, fragmentShaderSrc, size);

  int success;
  char infoLog[512];
      
  glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
  glCompileShader(vertexShader);
  free(vertexShaderSrc);
  
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    Output::stream << "Vertex shader compilation failed\n";
    Output::stream << infoLog;
    Output::stream << "\n";
    return 0;
  }
      
  glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
  glCompileShader(fragmentShader);
  free(fragmentShaderSrc);
  
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    Output::stream << "Fragment shader compilation failed\n";
    Output::stream << infoLog;
    Output::stream << "\n";
    return 0;
  }
      
  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glGetProgramiv(program, GL_LINK_STATUS, &success);
  
  if (!success){
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    Output::stream << "Shader program linking failed\n";
    Output::stream << infoLog;
    Output::stream << "\n";
    return 0;
  }

  return program;
}
