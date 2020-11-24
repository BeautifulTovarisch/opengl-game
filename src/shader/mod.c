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
    GLint info_log_len = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 1) {
      char *info_log = (char *)malloc(sizeof(char) * info_log_len);
      glGetShaderInfoLog(shader, info_log_len, NULL, info_log);

      fprintf(stderr, "Error compiling shader %s: %s\n", file, info_log);
      free(info_log);
    }

    glDeleteShader(shader);
    return 0;
  }

  return shader;
}
