#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <string>
#include "../core/transform.hpp"
#include "../physics/collision.hpp"

class Object {
  public:
    Object(const char* obj, const char* texture);
    ~Object();
    void Update();
    void ShowCollider();
    void HideCollider();
    
    Physics::AABB& Collider();
    const Physics::AABB& Collider() const;
    void Collider(Physics::AABB collider);
    Transform transform;
    const int renderableID;
  private:
    void UpdateGraphics();
    Physics::AABB collider = Physics::AABB(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    int colliderRenderableID = -1;
};

#endif