#include "core/graphics.hpp"
#include "core/input.hpp"
#include <iostream>

int main()
{
  static bool quit = false;

  Graphics::Init(800, 600);
  Input::Init();

  int triangle1 = Graphics::CreateTriangle();
  Graphics::TranslateTriangle(triangle1, 0.5f, 0.0f, 0.0f);
  
  while (!quit && !Graphics::WindowShouldClose()){
    if (Input::GetKey(GLFW_KEY_ESCAPE)){
      quit = true;
    }
    Graphics::Update();
  }

  Input::Quit();
  Graphics::Quit();
  return 0;
}
