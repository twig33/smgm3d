#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm/glm.hpp>
#include "map.hpp"

extern const glm::vec3 globalForward;
extern const glm::vec3 globalRight;
extern const glm::vec3 globalUp;

class Transform {
public:
  glm::mat4 LocalToWorld() const;
  glm::mat4 WorldToLocal() const;
  glm::mat4 GetGlobalOrientation() const;
  
  glm::vec3 GetForward() const;
  glm::vec3 GetRight() const;
  glm::vec3 GetUp() const;
  
  void SetParent(Transform* parent);
  
  void SetPosition (glm::vec3 p);
  glm::vec3 GetPosition() const;
  void SetEulerAngles (glm::vec3 r);
  glm::vec3 GetEulerAngles() const;
  void SetScale (glm::vec3 s);
  glm::vec3 GetScale() const;

  void Translate(glm::vec3 t);
  void RotateEulerAngles(glm::vec3 r, bool localOrientation);
  void Scale(glm::vec3 s);
private:
  void AddChild(Transform* child); //this doesn't perform any safety checks to prevent pointer loops
  
  void UpdateModel(bool updateLocal = true);
  long GetChildIndex(Transform* p);
  
  std::vector<Transform*> children;
  Transform* parent = NULL;
  
  glm::mat4 localModel = glm::mat4(1.0f); //transformations relative to parent
  glm::mat4 model = glm::mat4(1.0f);
  
  glm::mat4 positionMat = glm::mat4(1.0f);
  glm::mat4 orientationMat = glm::mat4(1.0f);
  glm::mat4 globalOrientationMat = glm::mat4(1.0f);
  glm::mat4 scaleMat = glm::mat4(1.0f);
  
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
  
  glm::vec3 forward = globalForward;
  glm::vec3 right = globalRight;
  glm::vec3 up = globalUp;
};

#endif
