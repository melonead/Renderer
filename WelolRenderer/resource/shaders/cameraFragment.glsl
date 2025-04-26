#version 430 core

out vec4 color;
in vec2 fragTexCoord;

uniform sampler2D cameraTexture;

void main()
{
    color = texture(cameraTexture, fragTexCoord);
    //color = vec4(fragTexCoord.x, fragTexCoord.y, 0.0, 1.0);
}