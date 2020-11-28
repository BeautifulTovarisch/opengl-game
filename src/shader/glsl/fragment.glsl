#version 330 core
in vec2 tex_coord;
out vec4 frag_color;

// texture sampler
uniform vec3 sprite_color;
uniform sampler2D image;


void main()
{
	frag_color = vec4(sprite_color, 1.0) * texture(image, v_tex_coord);
}