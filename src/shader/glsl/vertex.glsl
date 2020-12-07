#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 tex_coord;

uniform mat4 model;
uniform vec4 rotation;
uniform mat4 projection;

vec3 rotate(vec4 q, vec3 pos) {
  return pos + 2.0 * cross(q.xyz, cross(q.xyz, pos) + q.w * pos);
}

void main()
{
  tex_coord = vec2(vertex.z, vertex.w);
  vec3 pos = rotate(rotation, vertex.xyz);

  gl_Position = vec4(pos, 1.0) * model * projection;
}