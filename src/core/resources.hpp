#ifndef RESOURCES_HPP_
#define RESOURCES_HPP_

#include "resourceids.hpp"
#include <glad/glad.h>

namespace Resources {
  GLuint GetVBO(unsigned int index);
  GLuint GetTexture(unsigned int index);
  int Load();
  int Free();
}

#endif
