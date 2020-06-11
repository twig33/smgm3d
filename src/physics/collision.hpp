#ifndef COLLISION_HPP_
#define COLLISION_HPP_

#include <glm/glm.hpp>

class Object; //forward declaration

namespace Physics {
  class AABB {
    public:
      AABB(glm::vec3 dimensions, glm::vec3 centerPoint = glm::vec3(0.0f, 0.0f, 0.0f));
      AABB(bool compensateRotations, glm::vec3 dimensions, glm::vec3 centerPoint = glm::vec3(0.0f, 0.0f, 0.0f));
	  
      void Transform(glm::mat4 model, glm::vec3 scale);
      /* When cube.obj is transformed with this it represents the collision box */
      glm::mat4 CubeMat() const;
      bool IsColliding(const AABB& b) const;
      glm::vec3 PointA() const;
      glm::vec3 PointB() const;
      glm::vec3 Dimensions() const;
    private:
	  bool dynamic = false;
	  
      glm::vec3 initialPoint;
      glm::vec3 initialDimensions;
      
      glm::vec3 point = initialPoint;
      glm::vec3 dimensions = initialDimensions;
  };
  bool IsColliding(const Object& a, const Object& b);
}

#endif