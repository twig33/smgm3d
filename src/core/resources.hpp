#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include <vector>
#include <map>
#include <string>
#include <glad/glad.h>

namespace Resources {
  struct MeshData {
    GLuint VBO;
    GLuint EBO;
    unsigned long numIndices;
  };
  static const char* const resourcesCFGPath = "resources.cfg";
  
  const std::vector<std::string>& MeshNames();
  const std::vector<std::string>& TextureNames();
  unsigned long MeshCount();
  unsigned long TextureCount();
  MeshData GetMeshData(std::string name);;
  GLuint GetTexture(std::string name);
  int Load();
  int Free();
}

#endif
