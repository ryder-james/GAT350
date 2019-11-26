#version 430
	
layout (location = 0) in vec3 i_position;
layout (location = 1) in vec3 i_normal;
layout (location = 3) in vec2 i_texcoord;
layout (location = 5) in vec3 i_tangents;

out vec3 fposition;
out vec3 fnormal;
out vec2 ftexcoord;
out mat3 ftbn_matrix;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

void main() {
	fposition = vec3(mv_matrix * vec4(i_position, 1.0));
	fnormal = normalize(mat3(mv_matrix) * i_normal);
	ftexcoord = i_texcoord;

	vec3 tangent = normalize(mat3(mv_matrix) * i_normal);
	vec3 bitangent = normalize(cross(fnormal, tangent));

	ftbn_matrix = mat3(tangent, bitangent, fnormal);

	gl_Position = mvp_matrix * vec4(i_position, 1.0);
}
