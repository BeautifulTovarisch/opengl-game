#include "mod.h"

const char *const *open_shader(const char *src) {
  FILE *fp = fopen(src, "r");

  if (!fp) {
    char msg[256] = {0};
    snprintf(msg, 256, "Could not open shader file: %s", src);
    Log(ERROR, msg);

    return NULL;
  }

  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);
  rewind(fp);

  char *buffer = malloc(size);

  if (!buffer) {
    fclose(fp);
    Log(ERROR, "Failure to allocate memory for buffer.");

    return NULL;
  }

  if (fread(buffer, size, 1, fp) != 1) {
    fclose(fp);
    free(buffer);
    Log(ERROR, "Failure reading shader file contents");

    return NULL;
  }

  fclose(fp);

  // Remember to free when finished compiling!
  return (const char *const *)buffer;
}

GLuint Shader_Compile(GLenum type, const char *file, GLuint prog) {
  GLuint shader = glCreateShader(type);

  const char *const *shader_src = open_shader(file);

  if (shader_src == NULL) {
    char msg[256] = {0};
    snprintf(msg, sizeof(msg), "Unable to read shader source %s", file);

    Log(ERROR, msg);
    return 0;
  }

  glShaderSource(shader, 1, &shader_src, NULL);
  glCompileShader(shader);

  // Cast const to void in order to free
  free((void *)shader_src);

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

  if (!compiled) {
    Logger_GetLogInfo("Failure compiling shader", shader, 1);
    glDeleteShader(shader);
    return 0;
  }

  glAttachShader(prog, shader);

  return shader;
}

int Shader_Link(GLint gl_program) {
  int linked;

  glLinkProgram(gl_program);
  glGetProgramiv(gl_program, GL_LINK_STATUS, &linked);

  if (!linked) {
    Logger_GetLogInfo("Failure linking program", gl_program, 0);
    glDeleteProgram(gl_program);

    return 0;
  }

  return 1;
}
