#include "object.hpp"

#include <string>
#include <glm/glm.hpp>
#include "../core/graphics.hpp"

Object::Object(const char* obj, const char* texture) : renderableID(Graphics::CreateRenderable(obj, texture)){
  Update();
}

void Object::Update(){
  transform.Apply();
  collider.Transform(transform.LocalToWorld());
  UpdateGraphics();
}

void Object::UpdateGraphics(){
  Graphics::UpdateModel(renderableID, transform.LocalToWorld());
  if (colliderRenderableID != -1){
    Graphics::UpdateModel(colliderRenderableID, collider.CubeMat());
  }
}

Object::~Object(){
  Graphics::DestroyRenderable(renderableID);
}

Physics::AABB& Object::Collider(){
  return collider;
}

const Physics::AABB& Object::Collider() const {
  return collider;
}

void Object::Collider(Physics::AABB a){
  collider = a;
  Update();
}
void Object::ShowCollider(){
  if (colliderRenderableID != -1){
    return;
  }
  colliderRenderableID = Graphics::CreateRenderable("cube", "collider");
  Graphics::UpdateModel(colliderRenderableID, collider.CubeMat());
}

void Object::HideCollider(){
  if (colliderRenderableID == -1){
    return;
  }
  Graphics::DestroyRenderable(colliderRenderableID);
  colliderRenderableID = -1;
}