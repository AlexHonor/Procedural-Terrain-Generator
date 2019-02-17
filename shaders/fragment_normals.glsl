#version 330 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

void main()
{
  color = vec4(vNormal * 0.5f + vec3(0.5f, 0.5f, 0.5f), 1.0f);
}