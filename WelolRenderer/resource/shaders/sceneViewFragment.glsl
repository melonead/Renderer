#version 430 core
out vec4 color;

in vec3 fragColor;

void main()
{	
	color = vec4(fragColor, 1.0f);
}