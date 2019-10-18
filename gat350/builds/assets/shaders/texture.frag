#version 430
	
in vec3 fcolor;
in vec2 ftexcoord;

out vec4 color;

layout (binding = 0) uniform sampler2D texture_sample;

void main() {
	color = texture(texture_sample, ftexcoord);//vec4(fcolor, 1.0);
}
