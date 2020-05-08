#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <GLFW/glfw3.h>

namespace Graphics {
  int CreateTriangle();
  void TranslateTriangle(int id, float x, float y, float z);
  
  GLFWwindow* GetWindow();
  int WindowShouldClose();
  int Update();
  void SetClearColor(float r, float g, float b, float a);
  bool Inited();
  int Init(unsigned int windowWidth, unsigned int windowHeight);
  int Quit();
}

#endif
