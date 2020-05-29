#include <vector>

#include "core/graphics.hpp"
#include "core/transform.hpp"
#include "core/input.hpp"
#include "core/output.hpp"
#include "core/util/mysleep.h"

#include "physics/collision.hpp"

#include "game/object.hpp"

int main()
{
  static bool quit = false;
  static const float turningSpeed = 70.0f;
  static const float cameraSpeed = 5.0f;
  static const float g = 0.04f;
  static const float jumpVo = 0.01f;
  static const float gamerSpeed = 5.0f;
  static const glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

  Graphics::Init(800, 600);
  Graphics::SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  Input::Init();

  Object gamer("gamer", "knuckles-cracked");
  gamer.transform.Scale(scale);
  gamer.Collider(Physics::AABB(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 4.0f, 2.0f)));
  Graphics::Camera().Parent(&gamer.transform);
  Graphics::Camera().Translate(glm::vec3(0.0f, 7.0f, 10.0f));
  
  Object plane2("plane", "default");
  plane2.transform.Scale(glm::vec3(10.0f, 1.0f, 10.0f));
  plane2.transform.Translate(glm::vec3(0.0f, -5.0f, 0.0f));
  plane2.Collider(Physics::AABB(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 1.0f, 2.0f)));
  plane2.Update();
  
  Object plane("plane", "concrete");
  plane.transform.Scale(glm::vec3(10.0f, 1.0f, 10.0f));
  plane.transform.Rotation(Quaternion::AngleAxis(-90.0f, globalRight));
  plane.transform.Translate(glm::vec3(0.0f, 0.0f, -20.0f));
  plane.Update();
  
  float lastTime = glfwGetTime() - 1.0f/60.0f;
  
  float cameraPitch = -25.0f;
  float gamerYaw = 0.0f;
  float gamerSpeedY = 0.0f;
  
  //for determining whether to show or hide colliders on f12 press
  bool toggle = true;
  
  while (!quit && !Graphics::WindowShouldClose()){
    /* Calculate deltatime */
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();
	
	//rotate gamer
	gamerYaw += Input::GetAxis(GLFW_KEY_LEFT, GLFW_KEY_RIGHT) * turningSpeed * dt;
	Quaternion qGamerYaw = Quaternion::AngleAxis(gamerYaw, globalUp);
	gamer.transform.LocalRotation(qGamerYaw);
	
    //move gamer
	bool grounded = false;
	if (Physics::IsColliding(gamer, plane2)){
		grounded = true;
	}
    if (!grounded){
		gamerSpeedY -= g * dt;
	}
	else {
		gamerSpeedY = 0.0f;
		if (Input::GetKeyDown(GLFW_KEY_SPACE)){
			gamerSpeedY = jumpVo;
		}
	}
	gamer.transform.Translate(Input::GetAxis(GLFW_KEY_W, GLFW_KEY_S) * dt * gamerSpeed * gamer.transform.Forward());
	gamer.transform.Translate(Input::GetAxis(GLFW_KEY_D, GLFW_KEY_A) * dt * gamerSpeed * gamer.transform.Right());
	gamer.transform.Translate(gamerSpeedY * gamer.transform.Up());	
	//apply
	gamer.Update();
	
	//rotate camera
	cameraPitch += Input::GetAxis(GLFW_KEY_UP, GLFW_KEY_DOWN) * turningSpeed * dt;
	if (cameraPitch > 20.0f)
      cameraPitch = 20.0f;
	if (cameraPitch < -45.0f)
      cameraPitch = -45.0f;
	
	Graphics::Camera().LocalRotation(Quaternion::AngleAxis(cameraPitch, globalRight));
	
	//apply
	Graphics::Camera().Apply();
	
	if (Input::GetKeyDown(GLFW_KEY_F12)){
      if (toggle){
		  gamer.ShowCollider();
		  plane2.ShowCollider();
	  }
	  else {
		  gamer.HideCollider();
		  plane2.HideCollider();
	  }
      toggle = !toggle;
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
