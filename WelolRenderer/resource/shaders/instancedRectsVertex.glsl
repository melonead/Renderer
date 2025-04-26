#version 430 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 color;
layout (location=2) in vec2 displacement;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 fragColor;

void main()
{
	fragColor = color;
	gl_Position = projectionMatrix * viewMatrix * vec4(position.xy + displacement.xy, 0.0, 1.0);
}