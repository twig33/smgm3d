#include "transform.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::mat4& Transform::LocalToWorld() const {
  return model;
}

void Transform::UpdateModel() {
  model = positionMat * rotationMat * scaleMat;
}

void Transform::SetPosition (glm::vec3 p){
  position = p;
  positionMat = glm::translate(glm::mat4(1.0f), p);
  UpdateModel();
}

const glm::vec3& Transform::GetPosition() const {
  return position;
}

void Transform::SetEulerAngles (glm::vec3 r){
  rotation = r;
  rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(r.x), glm::vec3(1.0, 0.0, 0.0));
  rotationMat = glm::rotate(rotationMat, glm::radians(r.y), glm::vec3(0.0, 1.0, 0.0));
  rotationMat = glm::rotate(rotationMat, glm::radians(r.z), glm::vec3(0.0, 0.0, 1.0));
  UpdateModel();
}

const glm::vec3& Transform::GetRotation() const {
  return rotation;
}

void Transform::SetScale (glm::vec3 s) {
  scale = s;
  scaleMat = glm::scale(glm::mat4(1.0f), s);
  UpdateModel();
}

const glm::vec3& Transform::GetScale() const {
  return scale;
}

void Transform::Translate(glm::vec3 t) {
  position += t;
  positionMat = glm::translate(positionMat, t);
  UpdateModel();
}

void Transform::RotateEulerAngles(glm::vec3 r) {
  rotation += r;
  rotationMat = glm::rotate(rotationMat, glm::radians(r.x), glm::vec3(1.0, 0.0, 0.0));
  rotationMat = glm::rotate(rotationMat, glm::radians(r.y), glm::vec3(0.0, 1.0, 0.0));
  rotationMat = glm::rotate(rotationMat, glm::radians(r.z), glm::vec3(0.0, 0.0, 1.0));  
  UpdateModel();
}

void Transform::Scale(glm::vec3 s) {
  scale += s;
  scaleMat = glm::scale(scaleMat, s);
  UpdateModel();
}
