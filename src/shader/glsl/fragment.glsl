#version 330 core
in vec2 tex_coord;
out vec4 frag_color;

// texture sampler
uniform vec3 sprite_color;
uniform sampler2D image;

void main()
{
  frag_color = texture(image, tex_coord);
}