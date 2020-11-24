#version 330 core
attribute vec4 a_pos;
attribute vec2 a_tex_coord;
varying vec2 v_tex_coord;
void main() {
  gl_Position = a_pos;
  v_tex_coord = a_tex_coord;
}