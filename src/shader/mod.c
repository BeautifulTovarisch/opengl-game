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

GLuint Shader_Compile(GLenum type, const char *file) {
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
    char msg[256] = {0};
    snprintf(msg, 256, "Failure compiling shader %s", file);

    Logger_GetLogInfo(msg, shader, 1);
    glDeleteShader(shader);
    return 0;
  }

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

void Shader_SetInteger(GLuint prog, const char *name, int i) {
  glUniform1i(glGetUniformLocation(prog, name), i);
}

void Shader_SetMatrix4(GLuint prog, const char *name, Mat4 mat) {
  glUniformMatrix4fv(glGetUniformLocation(prog, name), 1, 0, mat);
}

void Shader_SetVector3(GLuint prog, const char *name, Vector v) {
  glUniform3f(glGetUniformLocation(prog, name), v.x, v.y, v.z);
}

void Shader_SetVector4(GLuint prog, const char *name, Vector v) {
  glUniform4f(glGetUniformLocation(prog, name), v.x, v.y, v.z, v.w);
}
