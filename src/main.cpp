#include <vector>

#include "core/graphics.hpp"
#include "core/transform.hpp"
#include "core/input.hpp"
#include "core/output.hpp"
#include "core/mysleep.h"
#include "core/resourceids.hpp"

static const int numTriangles = 10;

class Object {
public:
  Object(int triangle) : triangle(triangle) {};
  float speedX = 1.0f;
  float speedY = 0.7f;
  int triangle;
  Transform transform;
};
  
int main()
{
  static bool quit = false;

  Graphics::Init(800, 600);
  Graphics::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  Input::Init();

  std::vector<Object> objects;
  
  for (int i = 0; i < numTriangles; ++i){
    objects.push_back(Object(Graphics::CreateRenderable(Resources::MESH_SQUARE, Resources::TEXTURE_DEFAULT)));
    objects.back().speedX += 0.1f * i;
    objects.back().speedY -= 0.01f * i;
    Graphics::UpdateModel(objects[i].triangle, objects[i].transform.LocalToWorld());
  }
  
  float bound = 2.0f;

  float lastTime = glfwGetTime();
  int fpsReportFrameInterval = 500;
  
  while (!quit && !Graphics::WindowShouldClose()){
    /* Calculate deltatime */
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    if (dt > 0){
      Output::stream << "FPS " << 1/dt << '\n';
      fpsReportFrameInterval = 500;
    }
    
    for (int i = 0; i < numTriangles; ++i){
      if (objects[i].speedX > 0) {
	if (objects[i].transform.GetPosition().x > bound) {
	  objects[i].speedX = -objects[i].speedX;
	}
      }
      else {
	if (objects[i].transform.GetPosition().x < -bound) {
	  objects[i].speedX = -objects[i].speedX;
	}	
      }
      if (objects[i].speedY > 0) {
	if (objects[i].transform.GetPosition().y > bound) {
	  objects[i].speedY = -objects[i].speedY;
	}
      }
      else {
	if (objects[i].transform.GetPosition().y < -bound) {
	  objects[i].speedY = -objects[i].speedY;
	}	
      }
    
      objects[i].transform.Translate(glm::vec3(objects[i].speedX, objects[i].speedY, 0.0f) * dt);
      objects[i].transform.RotateEulerAngles(glm::vec3(10.0f, 5.0f, 0.0f) * objects[i].speedX * dt * 10.0f);
      Graphics::UpdateModel(objects[i].triangle, objects[i].transform.LocalToWorld());
    }
    if (Input::GetKey(GLFW_KEY_ESCAPE)){
      quit = true;
    }
    
    Graphics::Update();
  }

  Input::Quit();
  Graphics::Quit();
  return 0;
}
