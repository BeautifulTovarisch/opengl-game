#include "mod.h"

const char *const *open_shader(const char *src) {
  FILE *fp = fopen(src, "r");

  if (!fp) {
    fprintf(stderr, "Could not open shader file: %s\n", src);
    return NULL;
  }

  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);

  rewind(fp);

  char *buffer = malloc(size);

  if (!buffer) {
    fclose(fp);
    fprintf(stderr, "Failure to allocate memory for buffer.\n");
    return NULL;
  }

  if (fread(buffer, size, 1, fp) != 1) {
    fclose(fp);
    free(buffer);
    fprintf(stderr, "Failure reading shader file contents\n");
    return NULL;
  }

  fclose(fp);

  // Remember to free when finished compiling!
  return (const char *const *)buffer;
}

GLuint LoadShader(GLenum type, const char *file) {
  GLuint shader = glCreateShader(type);

  const char *const *shader_src = open_shader(file);

  if (shader_src == NULL) {
    fprintf(stderr, "Unable to read shader source.\n");
    return 0;
  }

  glShaderSource(shader, 1, &shader_src, NULL);
  glCompileShader(shader);

  // Cast const to void in order to free
  free((void *)shader_src);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    Logger_GetLogInfo("Error compiling shader", shader, 1);
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}
