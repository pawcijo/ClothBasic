#version 450 core
layout (location = 0) in vec4 aPos;

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;

void main()
{
    FragPos =  aPos.xyz;
  gl_Position = projection * view * transform * vec4(aPos.xyz, 1.0);
}

