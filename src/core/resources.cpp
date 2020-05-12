#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>

#include "resources.hpp"
#include "output.hpp"

namespace Resources {
  namespace {
    struct RawMeshData {
      unsigned long verticesSize;
      float* vertices;
      unsigned long indicesSize;
      unsigned int* indices;
    };

    static float triangleVertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, 1.0f,
				       0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				       0.0f, 0.5f, 0.0f, 1.0f, 0.0f};

    static unsigned int triangleIndices[] = {0, 1, 2};
    
    static float squareVertices[] = {0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // top right
				     0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
				     -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,// bottom left
				     -0.5f, 0.5f, 0.0f, 0.0f, 1.0f // top left 
    };
    
    static unsigned int squareIndices[] = {0, 1, 3,
					   1, 2, 3};

    RawMeshData rawMeshData[MESHES_SIZE] = {[MESH_TRIANGLE] = {sizeof(triangleVertices), triangleVertices,
							       sizeof(triangleIndices), triangleIndices},
					    [MESH_SQUARE] = {sizeof(squareVertices), squareVertices,
							     sizeof(squareIndices), squareIndices}};
    
    MeshData meshData[MESHES_SIZE];
    GLuint textures[TEXTURES_SIZE];

    static const char* texturePaths[TEXTURES_SIZE] = {[TEXTURE_DEFAULT] = "brickwall.jpg"};
    
    GLuint LoadTexture(const char* path){
      GLuint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      
    // set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // load image, create texture and generate mipmaps

      int width, height, nrChannels;
      unsigned char *data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb);
      Output::stream << path << " nrChannels " << nrChannels << '\n';
      if (!data){
	return 0;
      }
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(data);

      return texture;
    }
    
    int LoadTextures(){
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      
      textures[TEXTURE_DEFAULT] = LoadTexture(texturePaths[TEXTURE_DEFAULT]);
      if (!textures[TEXTURE_DEFAULT]){
	Output::stream << "Resources: Error couldn't load default texture\n";
	return 0;
      }
      for (int i = TEXTURE_DEFAULT+1; i < TEXTURES_SIZE; ++i){
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
      for (int i = 0; i < MESHES_SIZE; ++i){
	glGenBuffers(1, &meshData[i].VBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshData[i].VBO);
	glBufferData(GL_ARRAY_BUFFER, rawMeshData[i].verticesSize, rawMeshData[i].vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &meshData[i].EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshData[i].EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, rawMeshData[i].indicesSize, rawMeshData[i].indices, GL_STATIC_DRAW);

	meshData[i].numIndices = rawMeshData[i].indicesSize / sizeof(unsigned int);
      }
      
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      
      return 1;
    }

    int FreeTextures(){
      glDeleteTextures(TEXTURES_SIZE, textures);
      return 1;
    }

    int FreeMeshes(){
      
      return 1;
    }
  }

  MeshData GetMeshData(unsigned int index){
    return meshData[index];
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
