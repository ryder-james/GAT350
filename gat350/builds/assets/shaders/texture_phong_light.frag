#version 430
#define POINT 0
#define DIRECTION 1
#define SPOTLIGHT 2

in vec3 fposition;
in vec3 fnormal;
in vec2 ftexcoord;

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
	int type;
	float cutoff;
	float exponent;

	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec4 position;
};

uniform light_s light;

layout (binding = 0) uniform sampler2D texture_sample;

void phong(light_s in_light, vec3 position, vec3 normal, out vec3 ambient, out vec3 diffuse, out vec3 specular)
{
	vec3 vector_to_light;
	
	if (in_light.type == POINT || in_light.type == SPOTLIGHT) 
	{
		vector_to_light = normalize(vec3(in_light.position) - position);
	}
	else if (in_light.type == DIRECTION)
	{
		vector_to_light = normalize(-in_light.direction);
	}
	else
	{
		//vector_to_light = normalize();
	}

	// ambient
	ambient = in_light.ambient * material.ambient;

	diffuse = vec3(0);
	specular = vec3(0);

	float spot_intensity = 1;

	if (in_light.type == SPOTLIGHT)
	{
		float cos_angle = dot(in_light.direction, -vector_to_light);
		float angle = acos(cos_angle);

		if (angle > in_light.cutoff)
		{
			return;
		}

		spot_intensity = pow(cos_angle, in_light.exponent);
	}

	// diffuse
	float lDotN = max(0.0, dot(normal, vector_to_light));
	diffuse = ((material.diffuse * in_light.diffuse) * lDotN) * spot_intensity;

	// specular
	specular = vec3(0.0);
	if (lDotN > 0.0)
	{
		vec3 vector_to_view = normalize(-position.xyz);
		vec3 light_reflect = reflect(-vector_to_light, normal);
		float intensity = max(0.0, dot(vector_to_view, light_reflect));
		intensity = pow(intensity, material.shininess);
		specular = (in_light.specular * material.specular * intensity) * spot_intensity;
	}
}

void main()
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	phong(light, fposition, fnormal, ambient, diffuse, specular);

	color = vec4(ambient + diffuse, 1.0f) * texture(texture_sample, ftexcoord) + vec4(specular, 1.0);
}
