#include "mod.h"

static const char *geom_src = "src/shader/glsl/geom.glsl";
static const char *vertex_src = "src/shader/glsl/vertex.glsl";
static const char *fragment_src = "src/shader/glsl/fragment.glsl";

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

GLuint compile_shader(GLenum type, const char *file) {
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
    Logger_GetLogInfo("[Error]::Shader", shader, 1);
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

int link_program(GLint gl_program) {
  int linked;

  glLinkProgram(gl_program);
  glGetProgramiv(gl_program, GL_LINK_STATUS, &linked);

  if (!linked) {
    Logger_GetLogInfo("[Error]::Shader", gl_program, 0);
    glDeleteProgram(gl_program);

    return 0;
  }

  return 1;
}

// Load and compile shaders, link program object
void Shader_Init() {
  GLuint geom_shader = compile_shader(GL_GEOMETRY_SHADER, geom_src);
  GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_src);
  GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_src);

  GLuint gl_program = glCreateProgram();

  glAttachShader(gl_program, geom_shader);
  glAttachShader(gl_program, vertex_shader);
  glAttachShader(gl_program, fragment_shader);

  link_program(gl_program);

  // Shaders unecessary after linking
  glDeleteShader(geom_shader);
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
}
