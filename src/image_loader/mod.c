#include "mod.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

char *IL_Load(const char *filepath, int *width, int *height) {
  int comp;
  unsigned char *data = stbi_load(filepath, width, height, &comp, 4);

  if (data == NULL) {
    fprintf(stderr, "Error loading image: %s\n.", filepath);
    return '\0';
  }

  return (char *)data;
}

void IL_Cleanup(char *data) { stbi_image_free(data); }

GLuint IL_CreateTexture2D(int width, int height, char *data) {
  GLuint texture_id;

  glGenTextures(1, &texture_id);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  if (Logger_CheckGLErrors("Failed to apply texture parameters")) {
    return 0;
  };

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  if (Logger_CheckGLErrors("Failed to create image texture")) {
    return 0;
  };

  return texture_id;
}
