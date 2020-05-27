#include <vector>

#include "core/graphics.hpp"
#include "core/transform.hpp"
#include "core/input.hpp"
#include "core/output.hpp"
#include "core/util/mysleep.h"

#include "game/object.hpp"

int main()
{
  static bool quit = false;
  static const float turningSpeed = 70.0f;
  static const float cameraSpeed = 5.0f;
  static const float gamerSpeed = 5.0f;
  static const glm::vec3 scale = glm::vec3(0.3f, 0.3f, 0.3f);

  Graphics::Init(800, 600);
  Graphics::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  Input::Init();

  Object gamer("gamer", "knuckles-cracked");
  Object plane("plane", "default");
  Object plane2("plane", "concrete");
  
  plane.transform.Translate(glm::vec3(0.0f, -2.0f, 0.0f));
  plane.transform.Scale(glm::vec3(10.0f, 10.0f, 10.0f));
  plane.PushModelToGraphics();
  
  plane2.transform.Translate(glm::vec3(0.0f, 5.0f, -10.0f));
  plane2.transform.Scale(glm::vec3(10.0f, 10.0f, 10.0f));
  plane2.transform.Rotation(Quaternion::AngleAxis(90.0f, globalRight));
  plane2.PushModelToGraphics();
  
  gamer.transform.Scale(scale);
  gamer.PushModelToGraphics();
 
  Graphics::Camera().Parent(&gamer.transform);
  Graphics::Camera().Translate(glm::vec3(0.0f, 10.0f, 10.0f));
  
  float lastTime = glfwGetTime() - 1.0f/60.0f;
  
  float cameraPitch = -25.0f;
  float gamerYaw = 0.0f;
  
  while (!quit && !Graphics::WindowShouldClose()){
    /* Calculate deltatime */
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();
	
	//rotate camera anchor
	gamerYaw += Input::GetAxis(GLFW_KEY_LEFT, GLFW_KEY_RIGHT) * turningSpeed * dt;
	Quaternion qGamerYaw = Quaternion::AngleAxis(gamerYaw, globalUp);
	gamer.transform.LocalRotation(qGamerYaw);
	
	//rotate camera
	cameraPitch += Input::GetAxis(GLFW_KEY_UP, GLFW_KEY_DOWN) * turningSpeed * dt;
	if (cameraPitch > 20.0f)
		cameraPitch = 20.0f;
	if (cameraPitch < -45.0f)
		cameraPitch = -45.0f;
	
	//move gamer
	gamer.transform.Translate(Input::GetAxis(GLFW_KEY_W, GLFW_KEY_S) * dt * gamerSpeed * gamer.transform.Forward());
	gamer.transform.Translate(Input::GetAxis(GLFW_KEY_D, GLFW_KEY_A) * dt * gamerSpeed * gamer.transform.Right());
	
	gamer.PushModelToGraphics();
	
	Graphics::Camera().LocalRotation(Quaternion::AngleAxis(cameraPitch, globalRight));
    if (Input::GetKey(GLFW_KEY_ESCAPE)){
      quit = true;
    }
    
    Graphics::Update();
  }

  Input::Quit();
  Graphics::Quit();
  return 0;
}
