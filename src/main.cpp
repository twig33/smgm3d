#include "core/graphics.hpp"
#include "core/transform.hpp"
#include "core/input.hpp"
#include "core/output.hpp"
#include "core/mysleep.h"

int main()
{
  static bool quit = false;

  Graphics::Init(800, 600);
  Input::Init();

  Transform triangle1transform;
  int triangle1 = Graphics::CreateTriangle(&triangle1transform);

  float speed = 1.0f;
  float rightBound = 2.0f;
  float leftBound = -2.0f;

  float lastTime = glfwGetTime();

  while (!quit && !Graphics::WindowShouldClose()){
    /* Calculate deltatime */
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    if (speed > 0){
      if (triangle1transform.GetPosition().x > rightBound){
	speed = -speed;
      }
    }
    else {
      if (triangle1transform.GetPosition().x < leftBound){
	speed = -speed;
      }	
    }
    
    triangle1transform.Translate(glm::vec3(speed*dt, 0.0f, 0.0f));
    triangle1transform.RotateEulerAngles(glm::vec3(10.0f, 5.0f, 0.0f) * speed * dt * 10.0f);
    
    if (Input::GetKey(GLFW_KEY_ESCAPE)){
      quit = true;
    }
    
    Graphics::Update();
  }

  Input::Quit();
  Graphics::Quit();
  return 0;
}
