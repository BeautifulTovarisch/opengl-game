#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 tex_coord;

uniform mat4 model;
uniform vec4 rotation;
uniform mat4 projection;
uniform vec4 translation;

vec3 transform(vec4 real, vec4 dual, vec3 pos) {
  vec3 rotn = pos + 2.0 * cross(real.xyz, cross(real.xyz, pos) + (real.w * pos));
  return rotn + 2.0 * (real.w * dual.xyz - dual.w * real.xyz) + cross(real.xyz, dual.xyz);
}

void main()
{
  tex_coord = vec2(vertex.z, vertex.w);

  vec3 pos = transform(rotation, translation, vec3(vertex.xy, 0.0));

  gl_Position = vec4(pos, 1.0) * model * projection;
}