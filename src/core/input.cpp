#include "input.hpp"
#include "graphics.hpp"
#include <GLFW/glfw3.h>
#include <cassert>

namespace Input {

  namespace { //private
    static GLFWwindow* window;
	unsigned int width, height;
    
	bool firstFrame = true;
	double lastDeltaX = 0;
	double lastDeltaY = 0;
	
	/* A key is "pressed" when the key callback gets an GLFW_PRESS event */
	/* A key becomes "held" when GetKeyDown is called and it is pressed */
	enum KeyState {
		released = 0,
		pressed,
		held
	};
	
	int state[GLFW_KEY_LAST+1] = {KeyState::released};
	
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		double mouseNewX;
		double mouseNewY;
		glfwGetCursorPos(window, &mouseNewX, &mouseNewY);
		lastDeltaX = xpos - width/2;
		lastDeltaY = ypos - height/2;
		glfwSetCursorPos(Graphics::GetWindow(), width/2, height/2);
	}
	
	static void key_callback (GLFWwindow* window, int key, int scancode, int action, int mods){
	  	if (action == GLFW_PRESS){
			state[key] = KeyState::pressed;
		}
		else if (action == GLFW_RELEASE){
			state[key] = KeyState::released;
		}
	}
	
  }

  int GetAxis(int keyPositive, int keyNegative){
    bool keyP = Input::GetKey(keyPositive);
    bool keyN = Input::GetKey(keyNegative);
    if (keyP == keyN)
      return 0;
    return keyP ? 1 : -1;
  }

  void GetMouseDelta(double* dx, double* dy){
	*dx = lastDeltaX;
	*dy = lastDeltaY;
	lastDeltaX = 0;
	lastDeltaY = 0;
  }
  
  bool GetKey(int key){
	return (state[key] == KeyState::pressed || state[key] == KeyState::held);
  }
  
  bool GetKeyDown(int key){
    if (state[key] == KeyState::pressed){
      state[key] = KeyState::held;
	  return true;
	}	
	return false;
  }
  
  int Init(){
    assert(Graphics::Inited());

    window = Graphics::GetWindow();
	Graphics::GetWindowSize(&width, &height);
	glfwSetInputMode(Graphics::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//if (glfwRawMouseMotionSupported())
		//glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetKeyCallback(window, key_callback);
	
    return 1;
  }

  int Quit(){
    return 1;
  }
}
