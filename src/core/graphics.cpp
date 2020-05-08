#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "graphics.hpp"
#include "shader.hpp"
#include "output.hpp"

namespace Graphics {

  namespace { //private

    static bool inited = false;
    static GLFWwindow* window;

    struct Triangle {
      int id;
      glm::mat4 model = glm::mat4(1.0f);
    };

    std::vector<Triangle> triangles;
    
    float triangleVertices[] = {
      -0.5f, -0.5f, 0.0f,
       0.5f, -0.5f, 0.0f,
       0.0f, 0.5f, 0.0f
    };

    unsigned int triangleVAO;

    float squareVertices[] = {
      0.5f, 0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
     -0.5f, -0.5f, 0.0f,
     -0.5f, 0.5f, 0.0f
    };

    unsigned int squareIndices[] = {
      0, 1, 3,
      1, 2, 3
    };

    glm::mat4 model= glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    static Shader* shader;
    
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
      glViewport(0, 0, width, height);
    }
    
    static int InitWindow(unsigned int width, unsigned int height){
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
      
      window = glfwCreateWindow(width, height, "smgm3d", NULL, NULL);
      
      if (!window)
	return 0;

      /* Make the window's context current */
      glfwMakeContextCurrent(window);

      /* Set a callback for when the window gets resized */
      glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
      
      return 1;
    }

  }

  int CreateTriangle(){
    Triangle triangle;
    if (!triangles.empty()){
      triangle.id = triangles.back().id + 1;
    }
    else {
      triangle.id = 0;
    }
    triangles.push_back(triangle);
    return triangle.id;
  }

  void TranslateTriangle(int id, float x, float y, float z){
    int index = -1;
    for (int i = 0; i < triangles.size(); ++i){
      if (triangles[i].id == id){
	index = i;
	break;
      }
    }
    if (index == -1)
      return;
    triangles[index].model = glm::translate(triangles[index].model, glm::vec3(x, y, z));
  }
  
  int Init(unsigned int windowWidth, unsigned int windowHeight){
    static const std::string p = "Graphics::Init::";
    
    /* Initialize GLFW */
    if (!glfwInit()){
      Output::stream << p + " Couldn't initialize GLFW\n";
      return 0;
    }

    /* Create a windowed mode window and its OpenGL context */
    if (!InitWindow(windowWidth, windowHeight)){
      Output::stream << p + " Couldn't initialize the window\n";
      return 0;
    }
    
    /* Initialize GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      Output::stream << p + " Couldn't initialize GLAD\n";
      return 0;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    /* Initialize shader */
    try {
      shader = new Shader("shader.vs", "shader.fs");
    }
    catch (std::runtime_error){
      Output::stream << "Shader initialization failed\n";
      return 0;
    }
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO;
    glGenVertexArrays(1, &triangleVAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(triangleVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 
    
    inited = true;
    return 1;
  }
  
  bool Inited(){
    return inited;
  }
  
  GLFWwindow* GetWindow(){
    return window;
  }
  
  int WindowShouldClose(){
    return glfwWindowShouldClose(window);
  }

  void SetClearColor(float r, float g, float b, float a){
    glClearColor(r, g, b, a);
  }
  
  int Quit(){
    delete shader;
    glfwTerminate();
    return 1;
  }
  
  int Update(){
    /* Clear the screen */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw */
    glUseProgram(shader->program);
    glBindVertexArray(triangleVAO);
    for (int i = 0; i < triangles.size(); ++i){
      glUniformMatrix4fv(shader->modelLoc, 1, GL_FALSE, glm::value_ptr(triangles[i].model));
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    return 1;
  }


}
