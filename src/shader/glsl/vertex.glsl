#version 330 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec3 rotation;

out vec2 tex_coord;

uniform mat4 projection;

void main()
{
  tex_coord  vertex.zw;
  rotation = rotation + vertex.xy;
  gl_Position = projection * vec4(rotation, 1.0);
}