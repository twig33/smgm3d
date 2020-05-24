#ifndef QUATERNION_HPP_
#define QUATERNION_HPP_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Quaternion {
	public:
		Quaternion(glm::quat quat) : quat(quat) {} ;
		static Quaternion AngleAxis(float angle, glm::vec3 axis);
		
		glm::quat GLMQuat() const;
		glm::mat4 GLMMat4() const;
	private:
		glm::quat quat;
};

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs);

#endif