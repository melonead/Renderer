#version 430 core
out vec4 color;
in vec2 fragTexCoord;
uniform sampler2D diffuse;
void main()
{
  color = texture(diffuse, fragTexCoord);
}
