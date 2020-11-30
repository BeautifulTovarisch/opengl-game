#ifndef SHADERS_H
#define SHADERS_H

#include <stdio.h>
#include <stdlib.h>

#include <GLES2/gl2.h>

#include "../logger/mod.h"

int Shader_Link(GLint gl_program);
GLuint Shader_Compile(GLenum type, const char *file, GLuint prog);

#endif
