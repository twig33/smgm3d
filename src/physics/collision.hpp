#ifndef COLLISION_HPP_
#define COLLISION_HPP_

#include <glm/glm.hpp>

class Object; //forward declaration

namespace Physics {
  class AABB {
    public:
      AABB(glm::vec3 centerPoint, glm::vec3 dimensions);
      
      void Transform(glm::mat4 model);
      /* When a cube that has width, height and depth 1.0f is transformed using this matrix, it represents the collision box */
      glm::mat4 CubeMat() const;
      bool IsColliding(const AABB& b) const;
      glm::vec3 PointA() const;
      glm::vec3 PointB() const;
      glm::vec3 Dimensions() const;
    private:
      glm::vec3 initialPoint;
      glm::vec3 initialDimensions;
      
      glm::vec3 point = initialPoint;
      glm::vec3 dimensions = initialDimensions;
  };
  bool IsColliding(const Object& a, const Object& b);
}

#endif