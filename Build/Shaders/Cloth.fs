#version 430 core

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
void main()
{
 FragColor =  vec4(vec3(0,1,0), 1.0);
}
