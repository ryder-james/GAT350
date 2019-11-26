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


glm::vec3 math::calculate_tangent(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec2& uv0, const glm::vec2& uv1, const glm::vec2& uv2) {
	const int u = 0;
	const int v = 1;

	glm::vec3 edge1 = v1 - v0;
	glm::vec3 edge2 = v2 - v0;

	glm::vec2 edge1_uv = uv1 - uv0;
	glm::vec2 edge2_uv = uv2 - uv0;

	float f = 1.0f / ((edge1_uv[u] * edge2_uv[v]) - (edge1_uv[v] * edge2_uv[u]));

	glm::vec3 tangent;
	tangent.x = f * (edge2_uv[v] * edge1.x - edge1_uv[v] * edge2.x);
	tangent.y = f * (edge2_uv[v] * edge1.y - edge1_uv[v] * edge2.y);
	tangent.z = f * (edge2_uv[v] * edge1.z - edge1_uv[v] * edge2.z);
	tangent = glm::normalize(tangent);

	return tangent;
}

glm::vec3 math::calculate_normal(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
	glm::vec3 vec1 = v1 - v2;
	glm::vec3 vec2 = v1 - v3;

	return normalize(cross(vec1, vec2));
}

void math::transform(std::vector<glm::vec3>& vectors, glm::mat4& matrix) {
	for (glm::vec3& vec : vectors) {
		vec = glm::vec4(vec, 1.0f) * matrix;
	}
}

void math::transform(std::vector<glm::vec3>& vectors, glm::mat3& matrix) {
	for (glm::vec3& vec : vectors) {
		vec = vec * matrix;
	}
}
