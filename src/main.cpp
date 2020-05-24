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
  Object plane2(Graphics::CreateRenderable("plane", "concrete"));
  
  plane.transform.Translate(glm::vec3(0.0f, -2.0f, 0.0f));
  plane.transform.Scale(glm::vec3(10.0f, 10.0f, 10.0f));
  Graphics::UpdateModel(plane.renderableID, plane.transform.LocalToWorld());
  
  plane2.transform.Translate(glm::vec3(0.0f, 5.0f, -10.0f));
  plane2.transform.Scale(glm::vec3(10.0f, 10.0f, 10.0f));
  plane2.transform.Rotation(Quaternion::AngleAxis(90.0f, globalRight));
  
  Graphics::UpdateModel(plane2.renderableID, plane2.transform.LocalToWorld());
  
  gamer.transform.Scale(scale);
  hand.transform.Scale(scale);
  
  hand.transform.Parent(&gamer.transform);
  hand.transform.Translate(glm::vec3(2.0f, 0.0f, 0.0f));
  
  float lastTime = glfwGetTime() - 1.0f/60.0f;
  
  float cameraYaw = 0.0f;
  float cameraPitch = 0.0f;
  float gamerYaw = 0.0f;
  float gamerPitch = 0.0f;
  
  while (!quit && !Graphics::WindowShouldClose()){
    /* Calculate deltatime */
    float dt = glfwGetTime() - lastTime;
    lastTime = glfwGetTime();

    /* Update camera */
	//move
    Graphics::Camera().Translate(dt * cameraSpeed * Input::GetAxis(GLFW_KEY_W, GLFW_KEY_S) * Graphics::Camera().Forward());
    Graphics::Camera().Translate(dt * cameraSpeed * Input::GetAxis(GLFW_KEY_D, GLFW_KEY_A) * Graphics::Camera().Right());
	
	//rotate based on mouse input
    double dx, dy;
    Input::GetMouseDelta(&dx, &dy);
	cameraYaw += -dx * dt * turningSpeed;
	cameraPitch += -dy * dt * turningSpeed;
	
	if (cameraPitch > 90.0f)
		cameraPitch = 90.0f;
	if (cameraPitch < -90.0f)
		cameraPitch = -90.0f;
				   
	Quaternion qCameraPitch = Quaternion::AngleAxis(cameraPitch, globalRight);
	Quaternion qCameraYaw = Quaternion::AngleAxis(cameraYaw, globalUp);
	
	Graphics::Camera().Rotation(qCameraYaw * qCameraPitch);
    
    /* Rotate gamer */
	float gamerPitch, gamerYaw;
	gamerPitch += Input::GetAxis(GLFW_KEY_UP, GLFW_KEY_DOWN) * dt * turningSpeed;
	gamerYaw += Input::GetAxis(GLFW_KEY_LEFT, GLFW_KEY_RIGHT) * dt * turningSpeed;
	
	Quaternion qGamerPitch = Quaternion::AngleAxis(gamerPitch, globalRight);
	Quaternion qGamerYaw = Quaternion::AngleAxis(gamerYaw, globalUp);
	
	gamer.transform.Rotation(qGamerYaw * qGamerPitch);
	
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
