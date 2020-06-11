#include "collision.hpp"

#include <algorithm>
#include <glm/glm.hpp>

#include "../game/object.hpp"

namespace Physics {  
  namespace { //private
    glm::vec3 MinXYZ(glm::vec3 a, glm::vec3 b){
      return glm::vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }
    glm::vec3 MaxXYZ(glm::vec3 a, glm::vec3 b){
      return glm::vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }
    glm::vec3 MinXYZ(glm::vec3* arr, int size){
      glm::vec3 result = arr[0];
      for (int i = 1; i < size; ++i){
        result = MinXYZ(result, arr[i]);
      }
      return result;
    }
    glm::vec3 MaxXYZ(glm::vec3* arr, int size){
      glm::vec3 result = arr[0];
      for (int i = 1; i < size; ++i){
        result = MaxXYZ(result, arr[i]);
      }
      return result;
    }
  }
  
  AABB::AABB(glm::vec3 dimensions, glm::vec3 centerPoint){
    glm::vec3 halfDimensions = dimensions / glm::vec3(2.0f, 2.0f, 2.0f);
    initialPoint = centerPoint - halfDimensions;
    initialDimensions = dimensions;
  }
  
  AABB::AABB(bool dynamic, glm::vec3 dimensions, glm::vec3 centerPoint) : dynamic(dynamic) {
    glm::vec3 halfDimensions = dimensions / glm::vec3(2.0f, 2.0f, 2.0f);
    initialPoint = centerPoint - halfDimensions;
    initialDimensions = dimensions;
  }
  
  void AABB::Transform(glm::mat4 model, glm::vec3 scale){
	if (dynamic){
      glm::vec3 newPoints[4];
      newPoints[0] = initialPoint;
      newPoints[1] = initialPoint + initialDimensions;
      newPoints[2] = glm::vec3(newPoints[1].x, newPoints[1].y, newPoints[0].z);
      newPoints[3] = glm::vec3(newPoints[0].x, newPoints[0].y, newPoints[1].z);
      
      for(int i = 0; i < 4; ++i){
        newPoints[i] = model * glm::vec4(newPoints[i], 1.0f);
      }
      
      glm::vec3 min = MinXYZ(newPoints, 4);
      glm::vec3 max = MaxXYZ(newPoints, 4);
      
      point = min;
      dimensions = max - min;
	}
	else {
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), scale);
		dimensions = scaleMat * glm::vec4(initialDimensions, 1.0f);
		glm::vec3 halfDimensions = dimensions / glm::vec3(2.0f, 2.0f, 2.0f);
		glm::vec3 halfInitialDimensions = initialDimensions / glm::vec3(2.0f, 2.0f, 2.0f);
		point = initialPoint + halfInitialDimensions;
		point = model * glm::vec4(point, 1.0f);
		point = point - halfDimensions;
	}
  }
  
  bool AABB::IsColliding(const AABB& b) const {
    if (PointB().x <= b.PointA().x)
      return false;
    if (PointA().x >= b.PointB().x)
      return false;
    
    if (PointB().y <= b.PointA().y)
      return false;
    if (PointA().y >= b.PointB().y)
      return false;
    
    if (PointB().z <= b.PointA().z)
      return false;
    if (PointA().z >= b.PointB().z)
      return false;
    
    return true;
  }
  
  glm::vec3 AABB::PointA() const {
    return point;
  }
  
  glm::vec3 AABB::PointB() const {
    return point + dimensions;
  }
  
  glm::vec3 AABB::Dimensions() const {
    return dimensions;
  }
  
  glm::mat4 AABB::CubeMat() const {
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), point);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), dimensions);
    return translate * scale;
  }
  bool IsColliding(const Object& a, const Object& b){
    return a.Collider().IsColliding(b.Collider());
  }
}