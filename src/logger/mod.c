#include "mod.h"

char *map_gl_err(GLenum err) {
  switch (err) {
  case GL_INVALID_ENUM:
    return "Invalid enum";
  case GL_INVALID_VALUE:
    return "Invalid value";
  case GL_INVALID_OPERATION:
    return "Invalid operation";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "Invalid framebuffer operation";
  case GL_OUT_OF_MEMORY:
    return "Out of Memory";
  case GL_STACK_UNDERFLOW:
    return "Stack underflow";
  case GL_STACK_OVERFLOW:
    return "Stack overflow";
  default:
    return "No Error";
  }
}

int Logger_CheckGLErrors(const char *message) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    fprintf(stderr, "%s - OpenGL Error: %s\n", message, map_gl_err(err));
  }

  return err;
}

// type 0|1 - Program or shader, respectively
void Logger_GetLogInfo(const char *message, GLuint obj, int type) {
  GLint info_log_len = 0;

  if (type == 0) {
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &info_log_len);
  } else {
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &info_log_len);
  }

  GLchar *info_log = (GLchar *)malloc(sizeof(char) * info_log_len);

  if (type == 0) {
    glGetProgramInfoLog(obj, info_log_len, NULL, info_log);
  } else {
    glGetShaderInfoLog(obj, info_log_len, NULL, info_log);
  }

  if (info_log_len > 1) {
    fprintf(stderr, "%s: %s\n", message, info_log);
  }

  free(info_log);
}
