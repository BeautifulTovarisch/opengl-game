#include "mod.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

GLuint Texture_Create2D(const char *file) {
  GLuint texture_id;

  int width, height, comp;
  unsigned char *data = stbi_load(file, &width, &height, &comp, 4);

  if (!data) {
    Log(ERROR, "Error loading texture");
    return 0;
  }

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

  // Release image data after creating texture
  stbi_image_free(data);

  return texture_id;
}
