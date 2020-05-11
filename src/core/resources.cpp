#include "resources.hpp"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "output.hpp"

namespace Resources {
  namespace {
    GLuint VBOs[MESHES_SIZE];
    GLuint textures[TEXTURES_SIZE];

    static const char* texturePaths[TEXTURES_SIZE] = {[TEXTURE_DEFAULT] = "brickwall.png"};
    static const float vertexData[MESHES_SIZE][9] =
      {
       [MESH_TRIANGLE] =
       {-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f }
      };
    
    GLuint LoadTexture(const char* path){
      GLuint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      int width, height, nrChannels;
      unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
      if (!data){
	return 0;
      }
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(data);

      return texture;
    }
    
    int LoadTextures(){
      textures[TEXTURE_DEFAULT] = LoadTexture(texturePaths[TEXTURE_DEFAULT]);
      if (!textures[TEXTURE_DEFAULT]){
	Output::stream << "Resources: Error couldn't load default texture\n";
	return 0;
      }
      for (int i = 0; i < TEXTURES_SIZE; ++i){
	textures[i] = LoadTexture(texturePaths[i]);
	if (!textures[i]){
	  Output::stream << "Resources: Couldn't load texture with path " << texturePaths[i] << "\n";
	  Output::stream << "Falling back on default texture\n";
	  textures[i] = textures[TEXTURE_DEFAULT];
	}
      }
      return 1;
    }

    int LoadMeshes(){
      glGenBuffers(MESHES_SIZE, VBOs);
      for (int i = 0; i < MESHES_SIZE; ++i){
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData[i]), vertexData[i], GL_STATIC_DRAW);
      }
      return 1;
    }

    int FreeTextures(){
      glDeleteTextures(TEXTURES_SIZE, textures);
      return 1;
    }

    int FreeMeshes(){
      glDeleteBuffers(MESHES_SIZE, VBOs);
      return 1;
    }
  }

  GLuint GetVBO(unsigned int index){
    if (index >= MESHES_SIZE)
      return 0;
    
    return VBOs[index];
  }
  
  GLuint GetTexture(unsigned int index){
    if (index >= TEXTURES_SIZE)
      return 0;

    return textures[index];
  }
  
  int Load(){
    return LoadTextures() && LoadMeshes();
  }
  
  int Free(){
    return FreeTextures() && FreeMeshes();
  }
}
