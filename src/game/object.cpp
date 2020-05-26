#include "object.hpp"

#include <string>
#include "../core/graphics.hpp"

Object::Object(const char* obj, const char* texture) : renderableID(Graphics::CreateRenderable(obj, texture)){
	
}