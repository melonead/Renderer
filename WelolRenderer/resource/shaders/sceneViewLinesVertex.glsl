#version 430 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 position2;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 model;

uniform int lineSpacing;
uniform int numberOfLines;

void main()
{	
	int halfOfLinesCount = numberOfLines/2;
	if (gl_InstanceID < halfOfLinesCount)
	{
		gl_Position = projectionMatrix * viewMatrix * model * vec4(position.x, position.y, position.z + gl_InstanceID * lineSpacing, 1.0);
	}
	else {
		gl_Position = projectionMatrix * viewMatrix * model * vec4(position2.x + (gl_InstanceID-halfOfLinesCount) * lineSpacing, position2.y, position2.z, 1.0);
	}
	
}