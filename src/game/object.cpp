#include "object.hpp"

#include <string>
#include "../core/graphics.hpp"

Object::Object(const char* obj, const char* texture) : renderableID(Graphics::CreateRenderable(obj, texture)){
	
}

void Object::PushModelToGraphics(){
	Graphics::UpdateModel(renderableID, transform.LocalToWorld());
}