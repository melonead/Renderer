#version 430 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 fragColor;

void main()
{
	fragColor = color;
	gl_Position = projectionMatrix * viewMatrix * vec4(position, 1.0);
}