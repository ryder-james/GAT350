#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;
layout (location = 3) in vec2 vtexcoord;

out vec3 fposition;
out vec3 fnormal;
out vec2 ftexcoord;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

void main() {
	fnormal = normalize(mat3(mv_matrix) * vnormal);
	fposition = vec3(mv_matrix * vec4(vposition, 1.0));
	ftexcoord = vtexcoord;

	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
