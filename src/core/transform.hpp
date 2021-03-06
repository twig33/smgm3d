#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "quaternion.hpp"
#include "map.hpp"

extern const glm::vec3 globalForward;
extern const glm::vec3 globalRight;
extern const glm::vec3 globalUp;

class Transform {
public:
  void Apply();
  
  glm::mat4 LocalToWorld() const;
  glm::mat4 WorldToLocal() const;
  
  glm::vec3 Forward() const;
  glm::vec3 Right() const;
  glm::vec3 Up() const;
  
  glm::vec3 LocalPosition() const;
  glm::vec3 Position() const;
  glm::vec3 LocalEulerAngles() const;
  glm::vec3 EulerAngles() const;
  Quaternion LocalRotation() const;
  Quaternion Rotation() const;
  glm::vec3 LocalScale() const;
  glm::vec3 Scale() const;
  
  void Parent(Transform* parent);
  
  void LocalPosition (glm::vec3 p);
  void Position (glm::vec3 p);
  void Translate(glm::vec3 t);
  void LocalRotation(Quaternion quat);
  void Rotation(Quaternion quat);
  void LocalScale(glm::vec3 s);
  void Scale(glm::vec3 s);
private:
  void AddChild(Transform* child); //this doesn't perform any safety checks to prevent pointer loops
  
  void UpdateModel(bool updateLocal = true);
  long ChildIndex(Transform* p);
  
  std::vector<Transform*> children;
  Transform* parent = NULL;
  
  glm::mat4 localModel = glm::mat4(1.0f); //transformations relative to parent
  glm::mat4 model = glm::mat4(1.0f);
  
  glm::mat4 localPositionMat = glm::mat4(1.0f);
  glm::mat4 localScaleMat = glm::mat4(1.0f);
  
  glm::vec3 localPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  Quaternion localOrientation = Quaternion(glm::quat(1,0,0,0));
  Quaternion orientation = Quaternion(glm::quat(1,0,0,0));
  glm::vec3 localScale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  
  glm::vec3 forward = globalForward;
  glm::vec3 right = globalRight;
  glm::vec3 up = globalUp;
};

#endif
