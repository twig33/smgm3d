#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "transform.hpp"
#include "output.hpp"

static void EulerAnglesMod360(glm::vec3& eulerAngles){
  eulerAngles.x = fmod(eulerAngles.x, 360);
  eulerAngles.y = fmod(eulerAngles.y, 360);
  eulerAngles.z = fmod(eulerAngles.z, 360);
}

const glm::vec3 globalForward = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 globalRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 globalUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 Transform::GetForward() const {
	return forward;
}

glm::vec3 Transform::GetRight() const {
	return right;
}

glm::vec3 Transform::GetUp() const {
	return up;
}

glm::mat4 Transform::LocalToWorld() const {
  return model;
}

glm::mat4 Transform::WorldToLocal() const {
  return glm::inverse(model);
}

glm::mat4 Transform::GetGlobalOrientation () const {
	return globalOrientationMat;
}

void Transform::UpdateModel(bool updateLocal) {
  static glm::vec4 forward4, right4, up4;
  
  if (updateLocal){
    localModel = positionMat * orientationMat * scaleMat;
	forward4 = orientationMat * glm::vec4(globalForward, 1.0f);
	right4 = orientationMat * glm::vec4(globalRight, 1.0f);
	up4 = orientationMat * glm::vec4(globalUp, 1.0f);
  }
  
  if (parent){
    model = parent->LocalToWorld() * localModel;
	globalOrientationMat = parent->GetGlobalOrientation() * orientationMat;
	forward4 = parent->GetGlobalOrientation() * forward4;
	right4 = parent->GetGlobalOrientation() * right4;
	up4 = parent->GetGlobalOrientation() * up4;
  }
  else {
    model = localModel;
	globalOrientationMat = orientationMat;
  }

  forward = forward4;
  right = right4;
  up = up4;
  
  for (int i = 0; i < children.size(); ++i){
    children[i]->UpdateModel(false);
  }
}

void Transform::SetPosition (glm::vec3 p){
  position = p;
  positionMat = glm::translate(glm::mat4(1.0f), p);
  UpdateModel();
}

glm::vec3 Transform::GetPosition() const {
  return position;
}

void Transform::SetEulerAngles (glm::vec3 r){
  rotation = r;
  EulerAnglesMod360(rotation);
  orientationMat = glm::rotate(glm::mat4(1.0f), glm::radians(r.x), glm::vec3(1.0, 0.0, 0.0));
  orientationMat = glm::rotate(orientationMat, glm::radians(r.y), glm::vec3(0.0, 1.0, 0.0));
  orientationMat = glm::rotate(orientationMat, glm::radians(r.z), glm::vec3(0.0, 0.0, 1.0));
  UpdateModel();
}

glm::vec3 Transform::GetEulerAngles() const {
  return rotation;
}

void Transform::SetScale (glm::vec3 s) {
  scale = s;
  scaleMat = glm::scale(glm::mat4(1.0f), s);
  UpdateModel();
}

glm::vec3 Transform::GetScale() const {
  return scale;
}

void Transform::Translate(glm::vec3 t) {
  position += t;
  positionMat = glm::translate(positionMat, t);
  UpdateModel();
}

void Transform::RotateEulerAngles(glm::vec3 r, bool localOrientation) {
  rotation += r;
  EulerAnglesMod360(rotation);
  glm::mat4 neworientationMat;
  neworientationMat = glm::rotate(glm::mat4(1.0f), glm::radians(r.x), glm::vec3(1.0, 0.0, 0.0));
  neworientationMat = glm::rotate(neworientationMat, glm::radians(r.y), glm::vec3(0.0, 1.0, 0.0));
  neworientationMat = glm::rotate(neworientationMat, glm::radians(r.z), glm::vec3(0.0, 0.0, 1.0)); 
  if (localOrientation){
	orientationMat = orientationMat * neworientationMat;	
  }
  else {
	orientationMat = neworientationMat * orientationMat;
  }
  UpdateModel();
}

void Transform::Scale(glm::vec3 s) {
  scale += s;
  scaleMat = glm::scale(scaleMat, s);
  UpdateModel();
}

long Transform::GetChildIndex(Transform* p){
  for (std::size_t i = 0; i < children.size(); ++i){
    if (children[i] == p){
      return i;
    }
  }
  return -1;
}

void Transform::AddChild(Transform* p){
  if (GetChildIndex(p) != -1){
    return;
  }
  children.push_back(p);
  p->SetParent(this);
}

void Transform::SetParent(Transform* p){
  Transform* i = this;
  while ((i = i->parent) && i != NULL){
    if (i == p){
      return;
    }
  }
  
  parent = p;
  parent->AddChild(this);
}
