#include "input.hpp"
#include "graphics.hpp"
#include <GLFW/glfw3.h>
#include <cassert>

namespace Input {

  namespace { //private
    static GLFWwindow* window;

    bool firstFrame = true;
    double mouseOldX = 0;
    double mouseOldY = 0;
	double lastDeltaX = 0;
	double lastDeltaY = 0;
	
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
	{
		double mouseNewX;
		double mouseNewY;
		glfwGetCursorPos(window, &mouseNewX, &mouseNewY);
		if (!firstFrame){
		  lastDeltaX = mouseNewX - mouseOldX;
		  lastDeltaY = mouseNewY - mouseOldY;
		}
		else {
		  firstFrame = false;
		}
		mouseOldX = mouseNewX;
		mouseOldY = mouseNewY;
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
    return (glfwGetKey(window, key) == GLFW_PRESS);
  }
  
  int Init(){
    assert(Graphics::Inited());

    window = Graphics::GetWindow();
    glfwSetCursorPosCallback(window, cursor_position_callback);
	
    return 1;
  }

  int Quit(){
    return 1;
  }
}
