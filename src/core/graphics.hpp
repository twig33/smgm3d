#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <GLFW/glfw3.h>
#include "transform.hpp"

namespace Graphics {
  int CreateTriangle(const Transform * transform);
  
  GLFWwindow* GetWindow();
  int WindowShouldClose();
  int Update();
  void SetClearColor(float r, float g, float b, float a);
  bool Inited();
  int Init(unsigned int windowWidth, unsigned int windowHeight);
  int Quit();
}

#endif
