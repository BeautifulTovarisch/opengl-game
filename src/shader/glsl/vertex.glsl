#version 330 core
layout (location = 0) in vec4 vertex;

out vec2 tex_coord;

uniform mat4 model;
uniform vec4 rotation;
uniform mat4 projection;

void main()
{
  tex_coord = vec2(vertex.z, vertex.w);

  gl_Position = vec4(rotation.xyz, 1.0) * model * projection;
}