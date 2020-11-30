#include "mod.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

char *Texture_Load(const char *filepath, int *width, int *height) {
  int comp;
  unsigned char *data = stbi_load(filepath, width, height, &comp, 4);

  if (!data) {
    Log(ERROR, "Error loading texture");
    return '\0';
  }

  return (char *)data;
}

void Texture_Bind(GLuint texture_id) {
  glBindTexture(GL_TEXTURE_2D, texture_id);
}

GLuint Texture_Create2D(unsigned int width, unsigned int height, char *data) {
  GLuint texture_id;

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  if (Logger_CheckGLErrors("Failed to apply texture parameters")) {
    return 0;
  };

  // Release image data after creating texture
  stbi_image_free(data);

  return texture_id;
}
