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
  int num_errors = 0;
  GLenum err;
  while ((err = glGetError()) != GL_NO_ERROR) {
    num_errors++;
    fprintf(stderr, "%s - OpenGL Error: %s\n", message, map_gl_err(err));
  }

  return num_errors;
}
