#include "mod.h"

// TODO :: Standardize logging

const char *log_levels[] = {"TRACE",   "DEBUG", "INFO",
                            "WARNING", "ERROR", "FATAL"};

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

void format_time(char *buf) {
  time_t timer = time(NULL);
  struct tm *tm_info = localtime(&timer);

  strftime(buf, 25, "%Y-%m-%dT%H:%M:%S", tm_info);
}

void Log(Level level, const char *message) {
  // Allocate buffer the size of an ISO date string
  char time_buf[sizeof("1970-01-01T00:00:00Z")] = {0};

  format_time(time_buf);

  if (level >= ERROR) {
    fprintf(stdout, "[%s]::%s %s\n", log_levels[level], time_buf, message);
  } else {
    fprintf(stderr, "[%s]::%s %s\n", log_levels[level], time_buf, message);
  }
}

int Logger_CheckGLErrors(const char *message) {
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    char log[256] = {0};
    snprintf(log, sizeof(log), "OpenGL Error: %s", map_gl_err(err));

    Log(ERROR, log);
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
