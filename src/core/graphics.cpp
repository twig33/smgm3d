#include <map>
#include <vector>
#include <math.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics.hpp"
#include "resources.hpp"
#include "graphics/shader.hpp"
#include "output.hpp"
#include "map.hpp"

namespace Graphics {

  namespace { //private
	unsigned int windowWidth;
	unsigned int windowHeight;
	
    static bool inited = false;
    static GLFWwindow* window;

    class Renderable {
    public:
      Renderable(GLint VAO, GLuint texture, GLuint numIndices) : VAO(VAO), texture(texture), numIndices(numIndices) {};

      GLuint numIndices;
      GLuint VAO;
      GLuint texture;
      glm::mat4 model;
    };

    std::map<std::string, GLuint> VAOs;
    IntMap<Renderable> objects;

    Transform camera;
    
    /* Matrices */
    //camera
    //glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
    //				 glm::vec3(0.0f, 0.0f, 0.0f),
    //				 glm::vec3(0.0f, 1.0f, 0.0f));
    
    glm::mat4 projection = glm::perspective(glm::radians(75.0f), 1.0f, 0.01f, 10000.0f);

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
      
	  windowWidth = width;
	  windowHeight = height;
      return 1;
    }

  }

  void UpdateModel(int id, glm::mat4 model){
    objects.GetByKey(id).model = model;
  }
  
  int CreateRenderable(const char* mesh, const char* texture){
    return objects.Insert(Renderable(VAOs[mesh], Resources::GetTexture(texture), Resources::GetMeshData(mesh).numIndices));
  }

  void DestroyRenderable(int id){
    objects.EraseByKey(id);
  }

  Transform& Camera(){
    return camera;
  }
  
  void GetWindowSize(unsigned int* width, unsigned int* height){
	  *width = windowWidth;
	  *height = windowHeight;
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
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_CULL_FACE);
    //glfwSwapInterval(0);
    
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
    
    //glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    if (!Resources::Load()){
      Output::stream << "Graphics: Resource load failed\n";
      return 0;
    }

    for (int i = 0; i < Resources::MeshCount(); ++i){
      glGenVertexArrays(1, &VAOs[Resources::MeshNames()[i]]);
      glBindVertexArray(VAOs[Resources::MeshNames()[i]]);

      /* Bind buffers */
      glBindBuffer(GL_ARRAY_BUFFER, Resources::GetMeshData(Resources::MeshNames()[i]).VBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Resources::GetMeshData(Resources::MeshNames()[i]).EBO);

      /* Create the vertex attributes */
      /* 5 is kind of a magic number, I'll find a place to define it in a const variable later */
      /* 5 floats -- 3 for the vertex coord, 2 for the texture coord */
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
      
      glEnableVertexAttribArray(0);
      glEnableVertexAttribArray(1);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	

    camera.Position(glm::vec3(0.0f, 0.0f, 3.0f));

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
    Resources::Free();
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 1;
  }
  
  int Update(){
    /* Clear the screen */
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Set the view uniform */
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.WorldToLocal()));
    
    /* Draw */
    //glUseProgram(shaderProgram);
    for (int i = 0; i < objects.size(); ++i){
      glBindTexture(GL_TEXTURE_2D, objects[i].texture);
      glBindVertexArray(objects[i].VAO);
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(objects[i].model));
      glDrawElements(GL_TRIANGLES, objects[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
    return 1;
  }


}
