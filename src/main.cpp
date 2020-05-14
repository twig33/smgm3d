#include <vector>

#include "core/graphics.hpp"
#include "core/transform.hpp"
#include "core/input.hpp"
#include "core/output.hpp"
#include "core/mysleep.h"

class Object {
public:
  Object(int renderableID) : renderableID(renderableID) {};
  const int renderableID;
  Transform transform;
};
  
int main()
{
  static bool quit = false;
  static const float turningSpeed = 70.0f;
  static const glm::vec3 scale = glm::vec3(0.3f, 0.3f, 0.3f);
  
  Graphics::Init(800, 600);
  Graphics::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  Input::Init();

  Object gamer(Graphics::CreateRenderable("gamer", "knuckles-cracked"));
  Object hand(Graphics::CreateRenderable("cube", "default"));

  gamer.transform.SetScale(scale);
  hand.transform.SetScale(scale);

  gamer.transform.RotateEulerAngles(glm::vec3(0.0f, -90.0f, 0.0f));
  
  hand.transform.SetParent(&gamer.transform);
  hand.transform.Translate(glm::vec3(2.0f, 0.0f, 0.0f));
  hand.transform.RotateEulerAngles(glm::vec3(0.0f, -90.0f, 0.0f));
  
  float lastTime = glfwGetTime() - 1.0f/60.0f;
  
  while (!quit && !Graphics::WindowShouldClose()){
    /* Calculate deltatime */
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    if (Input::GetKey(GLFW_KEY_D)){
      gamer.transform.RotateEulerAngles(glm::vec3(0.0f, 0.0f, 1.0f) * -turningSpeed * dt);
    }
    else if (Input::GetKey(GLFW_KEY_A)){
      gamer.transform.RotateEulerAngles(glm::vec3(0.0f, 0.0f, 1.0f) * turningSpeed * dt); 
    }

    Graphics::UpdateModel(gamer.renderableID, gamer.transform.LocalToWorld());
    Graphics::UpdateModel(hand.renderableID, hand.transform.LocalToWorld());

    if (Input::GetKey(GLFW_KEY_ESCAPE)){
      quit = true;
    }
    
    Graphics::Update();
  }

  Input::Quit();
  Graphics::Quit();
  return 0;
}
