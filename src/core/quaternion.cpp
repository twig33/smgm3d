#include "quaternion.hpp"

Quaternion Quaternion::AngleAxis(float degrees, glm::vec3 axis){
	return Quaternion(glm::rotate(glm::quat(1, 0, 0, 0), glm::radians(degrees), glm::normalize(axis)));
}

glm::quat Quaternion::GLMQuat() const {
	return quat;
}

glm::mat4 Quaternion::GLMMat4() const {
	return glm::mat4_cast(quat);
}

Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs){
	return Quaternion(lhs.GLMQuat() * rhs.GLMQuat());
}