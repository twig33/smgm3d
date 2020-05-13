#include <vector>
#include <map>
#include <stdexcept>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "resources.hpp"
#include "output.hpp"

namespace Resources {
  namespace {
    struct RawMeshData {
      static const unsigned int vertexSize = 5 * sizeof(float);
      unsigned long verticesSize;
      float* vertices;
      unsigned long indicesSize;
      unsigned int* indices;
    };

    std::vector<std::string> meshNames;
    std::map<std::string, MeshData> meshData;
    
    std::vector<std::string> textureNames;
    std::map<std::string, GLuint> textures;
    
    GLuint CreateGLTexture(const char* path){
      GLuint texture;
      glGenTextures(1, &texture);
      glBindTexture(GL_TEXTURE_2D, texture);
      
      // set the texture wrapping parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      // set texture wrapping to GL_REPEAT (default wrapping method)
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
      // set texture filtering parameters
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      // load image, create texture and generate mipmaps

      int width, height, nrChannels;
      unsigned char *data = stbi_load(path, &width, &height, &nrChannels, STBI_rgb);
      
      if (!data){
	return 0;
      }
      
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);

      stbi_image_free(data);

      return texture;
    }

    /* Doesn't support quads */
    int LoadOBJ(RawMeshData* out, const char* path){
      std::vector<glm::vec3> vertexCoords;
      std::vector<glm::vec2> UVCoords;
      
      /* The reason this function needs the vertexToUVCoords map
	 is because opengl stores vertex data differently: the uv coordinates are contained in a vertex attribute, 
	 while the obj format stores them separately, referencing both vertex and uv coordinates in indices */
      std::map<unsigned int, unsigned int> vertexToUVCoords;
      std::vector<unsigned int> indices;
      
      std::ifstream file(path);
      if (!file)
	return 0;

      std::string line;
      
      while(std::getline(file, line)){
	std::istringstream iss(line);
	std::string header;
	iss >> header;

	//vertex coordinates
	if (header == "v"){
	  float x, y, z;
	  iss >> x >> y >> z;
	  vertexCoords.push_back(glm::vec3(x, y, z));
	}
	//uv coordinates
	else if (header == "vt"){
	  float x, y;
	  iss >> x >> y; 
	  UVCoords.push_back(glm::vec2(x, y));
	}
	//a triangle
	//one indice is formatted like x/y/... where x is the vertex coord num and y is the uv coord num, which start from 1
	else if (header == "f"){
	  for (int i = 0; i < 3; ++i) { //Take 3 indices
	    unsigned int v=0, vt=0;
	  
	    std::string indice;
	    iss >> indice;
	    
	    std::string vs, vts;
	    std::stringstream ss(indice);
	    
	    std::getline(ss, vs, '/');
	    v = std::stoi(vs);
	    std::getline(ss, vts, '/');
	    vt = std::stoi(vts);

	    /* If this vertex coordinate is already mapped to some other uv coordinate */
	    if (vertexToUVCoords.count(v-1) && vertexToUVCoords[v-1] != vt-1){
	      /* Create a new vertex with the same coordinates */
	      vertexCoords.push_back(vertexCoords[v-1]);
	      v = vertexCoords.size();
	    }
	    vertexToUVCoords[v-1] = vt-1;
	    indices.push_back(v-1);
	  }
	}
      }

      /* 5 is kind of a magic number but I'm pretty sure it won't change and i don't have any ideas where to define it yet. */
      /* 5 floats -- 3 for the vertex coord, 2 for the texture coord */
      out->verticesSize = vertexCoords.size() * 5 * sizeof(float);
      out->vertices = (float*)malloc(out->verticesSize);
      out->indicesSize = indices.size() * sizeof(unsigned int);
      out->indices = (unsigned int*)malloc(out->indicesSize);
      
      for (int i = 0; i < vertexCoords.size(); ++i){
	out->vertices[i*5] = vertexCoords[i].x;
	out->vertices[i*5+1] = vertexCoords[i].y;
	out->vertices[i*5+2] = vertexCoords[i].z;
	out->vertices[i*5+3] = UVCoords[vertexToUVCoords[i]].x;
	out->vertices[i*5+4] = UVCoords[vertexToUVCoords[i]].y;
      }

      std::copy(indices.begin(), indices.end(), out->indices);

      return 1;
    }

    void FreeOBJ(RawMeshData* data){
      free(data->vertices);
      free(data->indices);
    }

    int FreeTextures(){
      for (const auto& texture : textures){
	glDeleteTextures(1, &texture.second);
      }
      return 1;
    }

    int FreeMeshes(){
      for (const auto& mesh : meshData){
	glDeleteBuffers(1, &mesh.second.VBO);
	glDeleteBuffers(1, &mesh.second.EBO);
      }
      return 1;
    }
  }

  const std::vector<std::string>& MeshNames(){
    return meshNames;
  }

  const std::vector<std::string>& TextureNames(){
    return textureNames;
  }
  
  unsigned long MeshCount(){
    return meshData.size();
  }

  unsigned long TextureCount(){
    return textures.size();
  }

  MeshData GetMeshData(std::string name){
    auto pos = meshData.find(name);
    if (pos == meshData.end())
      throw std::logic_error("Invalid mesh name");
    
    return pos->second;
  }
  
  GLuint GetTexture(std::string name){
    auto pos = textures.find(name);
    if (pos == textures.end()){
      throw std::logic_error("Invalid texture name");
      return 0;
    }
    return pos->second;
  }

  /* Loads meshes and textures into dictionaries meshData and textures */
  int Load(){
    static const char* const resourcesCFGPath = "resources.cfg";
    static const char* const CFGTexturesToken = "textures:";
    static const char* const CFGMeshToken = "meshes:";
    
    std::ifstream cfg(resourcesCFGPath);
    if (!cfg){
      Output::stream << "Resources: Failed to load cfg\n";
      return 0;
    }
    
    std::string texturesPath, textureListFilename;
    std::string meshPath, meshListFilename;
    std::string line;
    
    while(std::getline(cfg, line)){
      std::istringstream iss(line);
      std::string buf;
      iss >> buf;
      if (buf == CFGTexturesToken){
        iss >> texturesPath >> textureListFilename;
      }
      else if (buf == CFGMeshToken){
	iss >> meshPath >> meshListFilename;
      }
    }
    
    std::ifstream objList(meshPath + meshListFilename);
    std::ifstream textureList(texturesPath + textureListFilename);
    if (!objList){
      Output::stream << "Resources: Failed to load the list of obj models\n";
      return 0;
    }
    if (!textureList){
      Output::stream << "Resources: Failed to load the list of textures\n";
      return 0;
    }

    /* Load meshes */
    std::string objName, objRelPath;
    while (objList >> objName >> objRelPath){
      RawMeshData rawData;
      if (!LoadOBJ(&rawData, (meshPath + objRelPath).c_str() )){
	Output::stream << "Resources: Failed to load obj model " << objName << '\n';
	continue;
      }
      meshNames.push_back(objName);
      
      MeshData data;
	
      glGenBuffers(1, &data.VBO);
      glBindBuffer(GL_ARRAY_BUFFER, data.VBO);
      glBufferData(GL_ARRAY_BUFFER, rawData.verticesSize, rawData.vertices, GL_STATIC_DRAW);

      glGenBuffers(1, &data.EBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, rawData.indicesSize, rawData.indices, GL_STATIC_DRAW);

      data.numIndices = rawData.indicesSize / sizeof(unsigned int);
      
      meshData.insert(std::make_pair(objName, data));

      FreeOBJ(&rawData);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Load Textures */
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //The textures were black-and-white without this line for some reason
    std::string texName, texRelPath;
    while (textureList >> texName >> texRelPath) {
      textures.insert(std::make_pair(texName, CreateGLTexture( (texturesPath + texRelPath).c_str() )));
      if (!textures[texName]){
	textures.erase(texName);
	Output::stream << "Resources: Failed to load texture " << texName << '\n';
	continue;
      }
      textureNames.push_back(texName);
    }

    return 1;
  }
  
  int Free(){
    return FreeTextures() && FreeMeshes();
  }
}
