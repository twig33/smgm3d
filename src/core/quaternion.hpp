#ifndef QUATERNION_HPP_
#define QUATERNION_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Quaternion {
	public:
		Quaternion(glm::quat quat) : quat(quat) {} ;
		Quaternion() {};
		static Quaternion AngleAxis(float degrees, glm::vec3 axis);
		
		glm::quat GLMQuat() const;
		glm::mat4 GLMMat4() const;
	private:
		glm::quat quat = glm::quat(1, 0, 0, 0);
};

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

#endif