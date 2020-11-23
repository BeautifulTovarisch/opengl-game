#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

#include <stdio.h>

#include <GLES2/gl2.h>

char *IL_Load(const char *filepath, int *width, int *height);

GLuint IL_CreateTexture2D(int width, int height, char *data);

#endif