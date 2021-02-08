#version 450 core

out vec4 FragColor;

in vec3 FragPos;

void main()
{

  float r,g,b;
  r=0;
  g=FragPos.z*.01;
  b=FragPos.z*.01;
  
  FragColor=vec4(vec3(r,g,b),1.f);
}
