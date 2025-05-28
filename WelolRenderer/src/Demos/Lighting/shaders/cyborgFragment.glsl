#version 430 core


out vec4 color;
in vec2 fragTexCoord;
uniform sampler2D diffuse;
// this is planet shader
void main()
{ 
    color = texture(diffuse, fragTexCoord);
    // color = vec4(1.0, 0.0, 0.0, 1.0);
}
