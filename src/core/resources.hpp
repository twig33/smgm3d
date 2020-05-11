#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include "resourceids.hpp"
#include <glad/glad.h>

namespace Resources {
  struct MeshData {
    GLuint VBO;
    GLuint EBO;
    unsigned long numIndices;
  };
  
  MeshData GetMeshData(unsigned int index);
  GLuint GetTexture(unsigned int index);
  int Load();
  int Free();
}

#endif
