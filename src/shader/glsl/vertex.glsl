#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 tex_coord;

uniform mat4 model;
uniform vec4 rotation;
uniform mat4 projection;
uniform vec4 translation;

vec3 rotate(vec4 real, vec3 pos) {
  vec3 qv = real.xyz;
  return qv * 2.0f * dot(qv, pos) + pos * (real.w * real.w - dot(qv, qv)) + cross(qv, pos) * 2.0f * real.w;
}

vec3 transform(vec4 real, vec4 dual, vec3 pos) {
  return rotate(real, pos) + dual.xyz;
}

void main()
{
  tex_coord = vec2(vertex.z, vertex.w);

  vec3 pos = transform(rotation, translation, vec3(vertex.xy, 0.0));

  gl_Position = projection * model * vec4(pos, 1.0);
}