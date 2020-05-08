#include "input.hpp"
#include "graphics.hpp"
#include <GLFW/glfw3.h>
#include <cassert>

namespace Input {

  namespace { //private
    static GLFWwindow* window;
  }

  bool GetKey(int key){
    return (glfwGetKey(window, key) == GLFW_PRESS);
  }
  
  int Init(){
    assert(Graphics::Inited());

    window = Graphics::GetWindow();
    
    return 1;
  }

  int Quit(){
    return 1;
  }
}
