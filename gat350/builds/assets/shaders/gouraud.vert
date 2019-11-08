#version 430
	
layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vnormal;

flat out vec3 fcolor;

struct material_s
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess; 
};

uniform material_s material;

struct light_s
{
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform light_s light;

uniform mat4 mv_matrix;
uniform mat4 mvp_matrix;

void main() {
	vec3 normal = normalize(mat3(mv_matrix) * vnormal);
	vec4 position = mv_matrix * vec4(vposition, 1.0);
	vec3 light_direction = normalize(vec3(light.position - position));

	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	float lDotN = max(0.0, dot(normal, light_direction));
	vec3 diffuse = light.diffuse * material.diffuse * lDotN;

	// specular
	vec3 specular = vec3(0.0);
	if (lDotN > 0.0)
	{
		vec3 position_to_view = normalize(-position.xyz);
		vec3 light_reflect = reflect(-light_direction, normal);
		float intensity = max(0.0, dot(position_to_view, light_reflect));
		intensity = pow(intensity, material.shininess);
		specular = light.specular * material.specular * intensity;
	}

	fcolor = ambient + diffuse + specular;

	gl_Position = mvp_matrix * vec4(vposition, 1.0);
}
