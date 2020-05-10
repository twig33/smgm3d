#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include <glm/glm.hpp>

class Transform {
public:
  const glm::mat4& LocalToWorld() const;

  void SetPosition (glm::vec3 p);
  const glm::vec3& GetPosition() const;
  void SetEulerAngles (glm::vec3 r);
  const glm::vec3& GetRotation() const;
  void SetScale (glm::vec3 s);
  const glm::vec3& GetScale() const;

  void Translate(glm::vec3 t);
  void RotateEulerAngles(glm::vec3 r);
  void Scale(glm::vec3 s);
private:
  void UpdateModel();
  
  glm::mat4 model = glm::mat4(1.0f);

  glm::mat4 positionMat = glm::mat4(1.0f);
  glm::mat4 rotationMat = glm::mat4(1.0f);
  glm::mat4 scaleMat = glm::mat4(1.0f);
  
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
};

#endif
