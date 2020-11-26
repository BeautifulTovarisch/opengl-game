#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <stdio.h>

#include <GLES2/gl2.h>

#include "../logger/mod.h"

void Texture_Cleanup(char *data);

char *Texture_Load(const char *filepath, int *width, int *height);

GLuint Texture_Create2D(int width, int height, char *data);

#endif
