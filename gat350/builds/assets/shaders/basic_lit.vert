#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;

out vec3 fcolor;

uniform vec3 ambient;
uniform vec4 light_pos;
uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

void main() {
	vec3 normal = normalize(mat3(mv_matrix) * vnormal);
	vec4 position = mv_matrix * vec4(vposition, 1.0f);

	vec3 light_direction = normalize(vec3(light_pos - position));

	float intensity = max(0.0f, dot(normal, light_direction));

	fcolor = ambient + intensity * vec3(0.0, 0.0, 1.0);
	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
