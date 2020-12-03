#ifndef SHADERS_H
#define SHADERS_H

#include <stdio.h>
#include <stdlib.h>

#include <GLES2/gl2.h>

#include "../linalg/mod.h"
#include "../logger/mod.h"

int Shader_Link(GLint gl_program);
GLuint Shader_Compile(GLenum type, const char *file);

void Shader_SetInteger(GLuint prog, const char *name, int i);
void Shader_SetMatrix4(GLuint prog, const char *name, Mat4 mat);

#endif
