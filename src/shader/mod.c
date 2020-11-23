#include "mod.h"

GLuint LoadShader(GLenum type, const char *shader_src) {
  GLuint shader = glCreateShader(type);

  glShaderSource(shader, 1, &shader_src, NULL);
  glCompileShader(shader);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    GLint info_log_len = 0;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_len);
    if (info_log_len > 1) {
      char *info_log = (char *)malloc(sizeof(char) * info_log_len);
      glGetShaderInfoLog(shader, info_log_len, NULL, info_log);

      fprintf(stderr, "Error compiling shader: %s\n", info_log);
      free(info_log);
    }

    glDeleteShader(shader);
    return 0;
  }

  return shader;
}
