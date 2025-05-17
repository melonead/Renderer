#version 430 core

out vec4 color;
in vec2 fragTexCoord;

uniform sampler2D cameraTexture;

void main()
{
    vec2 v = vec2(0.5, 0.5);
    color = texture(cameraTexture, v);
    //color = vec4(fragTexCoord.x, fragTexCoord.y, 0.0, 1.0);
}