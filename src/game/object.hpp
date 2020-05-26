#ifndef OBJECT_HPP_
#define OBJECT_HPP_

#include <string>
#include "../core/transform.hpp"

class Object {
	public:
		Object(const char* obj, const char* texture);
		Transform transform;
		const int renderableID;
};

#endif