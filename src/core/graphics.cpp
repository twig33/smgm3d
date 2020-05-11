#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics.hpp"
#include "resources.hpp"
#include "shader.hpp"
#include "output.hpp"
#include "map.hpp"

namespace Graphics {

  namespace { //private

    static bool inited = false;
    static GLFWwindow* window;

    class Renderable {
    public:
      Renderable(GLint VAO, GLuint texture) : VAO(VAO), texture(texture) {};
      
      GLuint VAO;
      GLuint texture;
      glm::mat4 model;
    };

    GLuint VAOs[Resources::MESHES_SIZE];
    IntMap<Renderable> objects;

    /* Matrices */
    //camera
    glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
				 glm::vec3(0.0f, 0.0f, 0.0f),
				 glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    /* Shader */
    static GLuint shaderProgram;
    static GLint modelLoc;
    static GLint viewLoc;
    static GLint projectionLoc;

    /* Callback for window resizing */
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
      /* Set the new width and height */
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

  void UpdateModel(int id, glm::mat4 model){
    objects.GetByKey(id).model = model;
  }
  
  int CreateRenderable(int mesh, int texture){
    if (mesh > Resources::MESHES_SIZE || texture > Resources::TEXTURES_SIZE)
      return 0;
    
    return objects.Insert(Renderable(VAOs[mesh], Resources::GetTexture(texture)));
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
    glfwSwapInterval(0);
    
    /* Initialize shader */
    shaderProgram = CreateShaderProgram("shader.vs", "shader.fs");
    if (!shaderProgram)
      return 0;

    modelLoc = GetUniformLocation(shaderProgram, "model");
    projectionLoc = GetUniformLocation(shaderProgram, "projection");
    viewLoc = GetUniformLocation(shaderProgram, "view");

    if (modelLoc == -1 || projectionLoc == -1 || viewLoc == -1)
      return 0;

    glUseProgram(shaderProgram);
    
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    if (!Resources::Load()){
      Output::stream << "Graphics: Resource load failed\n";
      return 0;
    }
    glGenVertexArrays(Resources::MESHES_SIZE, VAOs);

    for (int i = 0; i < Resources::MESHES_SIZE; ++i){
      glBindVertexArray(VAOs[i]);

      glBindBuffer(GL_ARRAY_BUFFER, Resources::GetVBO(i));

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindBuffer(GL_ARRAY_BUFFER, 0); 
      glBindVertexArray(0);
    }
    
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
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 1;
  }
  
  int Update(){
    /* Clear the screen */
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Draw */
    //glUseProgram(shaderProgram);
    for (int i = 0; i < objects.size(); ++i){
      glBindTexture(GL_TEXTURE_2D, objects[i].texture);
      glBindVertexArray(objects[i].VAO);
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(objects[i].model));
      glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    //return 1;
  }


}
