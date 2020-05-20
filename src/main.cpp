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
  static const float cameraSpeed = 5.0f;
  static const glm::vec3 scale = glm::vec3(0.3f, 0.3f, 0.3f);

  Graphics::Init(800, 600);
  glfwSetInputMode(Graphics::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  Graphics::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  Input::Init();

  Object gamer(Graphics::CreateRenderable("gamer", "knuckles-cracked"));
  Object hand(Graphics::CreateRenderable("cube", "default"));
  Object plane(Graphics::CreateRenderable("plane", "default"));
  
  plane.transform.Translate(glm::vec3(0.0f, -2.0f, 0.0f));
  plane.transform.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
  
  Graphics::UpdateModel(plane.renderableID, plane.transform.LocalToWorld());
  
  gamer.transform.SetScale(scale);
  hand.transform.SetScale(scale);

  gamer.transform.RotateEulerAngles(glm::vec3(0.0f, -90.0f, 0.0f), false);
  
  hand.transform.SetParent(&gamer.transform);
  hand.transform.Translate(glm::vec3(2.0f, 0.0f, 0.0f));
  hand.transform.RotateEulerAngles(glm::vec3(0.0f, -90.0f, 0.0f), false);
  
  float lastTime = glfwGetTime() - 1.0f/60.0f;
  
  while (!quit && !Graphics::WindowShouldClose()){
    /* Calculate deltatime */
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    /* Update camera */
	//move
    Graphics::Camera().Translate(dt * cameraSpeed * Input::GetAxis(GLFW_KEY_W, GLFW_KEY_S) * Graphics::Camera().GetForward());
    Graphics::Camera().Translate(dt * cameraSpeed * Input::GetAxis(GLFW_KEY_A, GLFW_KEY_D) * Graphics::Camera().GetLeft());
	
	//rotate based on mouse input
    double dx, dy;
    Input::GetMouseDelta(&dx, &dy);
	Graphics::Camera().RotateEulerAngles(glm::vec3(dt * -turningSpeed * dy,
	                                               0.0f,
												   0.0f), true);
    Graphics::Camera().RotateEulerAngles(glm::vec3(0.0f,
						                           dt * -turningSpeed * dx,
						                           0.0f), false);
    glm::vec3 rotation = Graphics::Camera().GetEulerAngles();
    if (rotation.x > 90.0f){
      rotation.x = 90.0f;
      Graphics::Camera().SetEulerAngles(rotation);
    }
    else if (rotation.x < -90.0f){
      rotation.x = -90.0f;
      Graphics::Camera().SetEulerAngles(rotation);
    }
    
    /* Rotate gamer */
    if (Input::GetKey(GLFW_KEY_DOWN)){
      gamer.transform.RotateEulerAngles(glm::vec3(0.0f, 0.0f, 1.0f) * -turningSpeed * dt, false);
    }
    else if (Input::GetKey(GLFW_KEY_UP)){
      gamer.transform.RotateEulerAngles(glm::vec3(0.0f, 0.0f, 1.0f) * turningSpeed * dt, false); 
    }

	/* Send model matrices to graphics */
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
