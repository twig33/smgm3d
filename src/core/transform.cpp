#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
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

void Transform::Apply(){
	UpdateModel();
}

glm::vec3 Transform::Forward() const {
	return forward;
}

glm::vec3 Transform::Right() const {
	return right;
}

glm::vec3 Transform::Up() const {
	return up;
}

glm::mat4 Transform::LocalToWorld() const {
  return model;
}

glm::mat4 Transform::WorldToLocal() const {
  return glm::inverse(model);
}

void Transform::UpdateModel(bool updateLocal) {
  glm::vec4 forward4, right4, up4;
  
  if (updateLocal){
    localModel = positionMat * localOrientation.GLMMat4() * scaleMat;
	forward4 = localOrientation.GLMMat4() * glm::vec4(globalForward, 1.0f);
	right4 = localOrientation.GLMMat4() * glm::vec4(globalRight, 1.0f);
	up4 = localOrientation.GLMMat4() * glm::vec4(globalUp, 1.0f);
  }
  
  if (parent){
    model = parent->LocalToWorld() * localModel;
	orientation = localOrientation * parent->Rotation();
	forward4 = parent->Rotation().GLMMat4() * forward4;
	right4 = parent->Rotation().GLMMat4() * right4;
	up4 = parent->Rotation().GLMMat4() * up4;
  }
  else {
    model = localModel;
	orientation = localOrientation;
  }

  forward = forward4;
  right = right4;
  up = up4;
  
  for (int i = 0; i < children.size(); ++i){
    children[i]->UpdateModel(false);
  }
}

void Transform::Position (glm::vec3 p){
  position = p;
  positionMat = glm::translate(glm::mat4(1.0f), p);
}

glm::vec3 Transform::Position() const {
  return position;
}

glm::vec3 Transform::EulerAngles() const {
  return glm::eulerAngles(orientation.GLMQuat());
}

glm::vec3 Transform::LocalEulerAngles() const {
  return glm::eulerAngles(localOrientation.GLMQuat());
}

void Transform::Scale (glm::vec3 s) {
  scale = s;
  scaleMat = glm::scale(glm::mat4(1.0f), s);
}

glm::vec3 Transform::Scale() const {
  return scale;
}

void Transform::Translate(glm::vec3 t) {
  position += t;
  positionMat = glm::translate(positionMat, t);
}

void Transform::LocalRotation(Quaternion quat){
  localOrientation = quat;
}

void Transform::Rotation(Quaternion quat){
  if (parent){
    Quaternion parentInverse = Quaternion(glm::quat_cast(glm::inverse(parent->Rotation().GLMMat4())));
    localOrientation = quat * parentInverse;
  }
  else {
    localOrientation = quat;
  }
}
  
Quaternion Transform::LocalRotation() const {
  return localOrientation;
}

Quaternion Transform::Rotation() const {
  return orientation;
}

long Transform::ChildIndex(Transform* p){
  for (std::size_t i = 0; i < children.size(); ++i){
    if (children[i] == p){
      return i;
    }
  }
  return -1;
}

void Transform::AddChild(Transform* p){
  if (ChildIndex(p) != -1){
    return;
  }
  children.push_back(p);
  p->Parent(this);
}

void Transform::Parent(Transform* p){
  Transform* i = this;
  while ((i = i->parent) && i != NULL){
    if (i == p){
      return;
    }
  }
  
  parent = p;
  parent->AddChild(this);
}
