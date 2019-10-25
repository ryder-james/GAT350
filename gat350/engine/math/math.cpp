#include "math.h"

float math::magnitude(const glm::vec3& v) {
	return sqrt(dot(v, v));
}

float math::distance(const glm::vec3& v1, const glm::vec3& v2) {
	return magnitude(v1 - v2);
}

float math::dot(const glm::vec3& v1, const glm::vec3& v2) {
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

glm::vec3 math::cross(const glm::vec3& v1, const glm::vec3& v2) {
	return glm::vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.z);
}

glm::vec3 math::normalize(const glm::vec3& v) {
	return v / magnitude(v);
}

glm::vec3 math::reflect(const glm::vec3& v, const glm::vec3& n) {
	return glm::vec3(0.0f);
}

glm::vec3 math::calculate_normal(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
	glm::vec3 vec1 = v1 - v2;
	glm::vec3 vec2 = v1 - v3;

	return normalize(cross(vec1, vec2));
}

void math::transform(std::vector<glm::vec3>& vectors, glm::mat4& matrix) {
	for (glm::vec3 &vec : vectors) {
		vec = glm::vec4(vec, 1.0f) * matrix;
	}
}

void math::transform(std::vector<glm::vec3>& vectors, glm::mat3& matrix) {
	for (glm::vec3& vec : vectors) {
		vec = vec * matrix;
	}
}
