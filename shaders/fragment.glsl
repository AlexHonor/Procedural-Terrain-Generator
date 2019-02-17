#version 330 core
in vec3 vFragPosition;
in vec2 vTexCoords;
in vec3 vNormal;

out vec4 color;

uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D sand;
uniform float coeff;

uniform sampler2D heightmap;

vec3 ComputeColor(float height) {
    vec4 col;

    float transition  = 0.08;
    float sand_level  = 0.10;
    float grass_level = 0.40;
    float rock_level  = 0.60;

    float grass_sand_border = (sand_level + grass_level) / 2.0f;
    float grass_rock_border = (rock_level + grass_level) / 2.0f;

    vec2 uv = vTexCoords * coeff;
    if (height < grass_sand_border - transition) {
        col = texture(sand, uv);
    } else if (height < grass_sand_border + transition) {
        height -= grass_sand_border - transition;
        height /= transition * 2;
        col = texture(sand, uv)  * (1.0f - height) +
              texture(grass, uv) * height;
    } else if (height < grass_rock_border - transition) {
        col = texture(grass, uv);    
    } else if (height < grass_rock_border + transition) {
        height -= grass_rock_border - transition;
        height /= transition * 2;
        col = texture(grass, uv) * (1.0f - height) +
              texture(rock, uv) * height;
    } else {
        col = texture(rock, uv);
    }

    height = clamp(height, 0, 1);
    
    return col.xyz;
}

void main()
{
  vec3 lightDir = vec3(1.0f, 1.0f, 0.0f); 

  float height = texture(heightmap, vTexCoords).x;
  vec3 col = ComputeColor(height);

  float kd = max(dot(vNormal, lightDir), 0.0);

  color = vec4(kd * col, 1.0f);
}