#version 430
	
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
	vec4 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform light_s light;

layout (binding = 0) uniform sampler2D texture_sample;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	vec3 vector_to_light = normalize(vec3(light.position) - fposition);

	// ambient
	vec3 ambient = light.ambient * material.ambient;

	// diffuse
	float lDotN = max(0.0, dot(fnormal, vector_to_light));
	vec3 diffuse = light.diffuse * material.diffuse * lDotN;
	diffuse = vec3(lDotN);

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

	color = vec4(ambient + diffuse, 1.0f) * texture(texture_sample, ftexcoord) + vec4(specular, 1.0);
	color = vec4(rgb2hsv(color.rgb), 1);
	color.r = 0.3;
	color = vec4(hsv2rgb(color.rgb), 1);
}
