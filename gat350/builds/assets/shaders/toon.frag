#version 430
	
in vec3 fposition;
in vec3 fnormal;

out vec4 color;
	
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
uniform int steps = 4;

float scale = 1.0 / steps;

vec3 toon(vec3 position, vec3 normal) {
	vec3 vector_to_light = normalize(vec3(light.position) - fposition);

	float lDotN = max(0.0, dot(fnormal, vector_to_light));
	vec3 diffuse = (material.diffuse * light.diffuse) * floor(lDotN * steps) * scale;

	return diffuse;
}

void main()
{
	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	vec3 diffuse = toon(fposition, fnormal);

	color = vec4(ambient + diffuse, 1.0);
}
