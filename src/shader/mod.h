#ifndef SHADERS_H
#define SHADERS_H

#include <stdio.h>
#include <stdlib.h>

#include <GLES2/gl2.h>

#include "../linalg/mod.h"
#include "../logger/mod.h"

int Shader_Link(GLint gl_program);
GLuint Shader_Compile(GLenum type, const char *file, GLuint prog);

void Shader_SetInteger(int i, GLuint prog, const char *name);
void Shader_SetMatrix4(Mat4 mat, GLuint prog, const char *name);

#endif
