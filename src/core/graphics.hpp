#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <GLFW/glfw3.h>
#include "transform.hpp"

namespace Graphics {
  int CreateRenderable(const char* mesh, const char* texture);
  void DestroyRenderable(int id);

  Transform& Camera();
  void GetWindowSize(unsigned int* width, unsigned int* height);
  void UpdateModel(int id, glm::mat4 model);
  GLFWwindow* GetWindow();
  int WindowShouldClose();
  int Update();
  void SetClearColor(float r, float g, float b, float a);
  bool Inited();
  int Init(unsigned int windowWidth, unsigned int windowHeight);
  int Quit();
}

#endif
