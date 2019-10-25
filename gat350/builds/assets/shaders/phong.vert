#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;

out vec3 fposition;
out vec3 fnormal;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

void main() {
	fnormal = normalize(mat3(mv_matrix) * vnormal);
	fposition = vec3(mv_matrix * vec4(vposition, 1.0));

	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
