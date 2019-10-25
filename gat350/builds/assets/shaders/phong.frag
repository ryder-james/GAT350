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

void main()
{
	vec3 vector_to_light = normalize(vec3(light.position) - fposition);

	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	float lDotN = max(0.0, dot(fnormal, vector_to_light));
	vec3 diffuse = light.diffuse * material.diffuse * lDotN;

	// specular
	vec3 specular = vec3(0.0);
	if (lDotN > 0.0)
	{
		vec3 vector_to_view = normalize(-fposition.xyz);
		vec3 light_reflect = reflect(-vector_to_light, fnormal);
		float intensity = max(0.0, dot(vector_to_view, light_reflect));
		intensity = pow(intensity, material.shininess);
		specular = light.specular * material.specular * intensity;
	}

	color = vec4(ambient + diffuse + specular, 1.0);
}
