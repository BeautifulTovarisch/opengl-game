#include "mod.h"

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

int link_program(GLint program_object) {
  int linked;

  glLinkProgram(program_object);
  glGetProgramiv(program_object, GL_LINK_STATUS, &linked);
  if (!linked) {
    GLint info_log_len = 0;
    glGetProgramiv(program_object, GL_INFO_LOG_LENGTH, &info_log_len);

    if (info_log_len > 1) {
      GLchar *info_log = (GLchar *)malloc(sizeof(char) * info_log_len);

      glGetProgramInfoLog(program_object, info_log_len, NULL, info_log);
      fprintf(stderr, "Error linking program: %s\n", info_log);
      free(info_log);
    }

    glDeleteProgram(program_object);
    return 0;
  }

  return 1;
}

GLFWwindow *init_window(int width, int height) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(width, height, "Hello Triangle", NULL, NULL);

  if (window == NULL) {
    fprintf(stderr, "Could not create window\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
    fprintf(stderr, "Failed to initialize GLAD\n");
    return NULL;
  }

  return window;
}

GLint Engine_Init() {
  GLbyte vector_shader_src[] = "#version 330 core\n"
                               "attribute vec4 a_pos;\n"
                               "attribute vec2 a_tex_coord;\n"
                               "varying vec2 v_tex_coord;\n"
                               "void main()\n"
                               "{\n"
                               "   gl_Position = a_pos;\n"
                               "   v_tex_coord = a_tex_coord;\n"
                               "}\0";

  GLbyte fragment_shader_src[] =
      "#version 330 core\n"
      "precision mediump float;\n"
      "varying vec2 v_tex_coord;\n"
      "uniform sampler2D s_texture;\n"
      "void main()\n"
      "{\n"
      "   gl_FragColor = texture2D(s_texture, v_tex_coord);\n"
      "}\n\0";

  GLint program_object = glCreateProgram();

  if (!program_object) {
    return 0;
  }

  // Compile and attach shaders
  GLuint vertex_shader =
      LoadShader(GL_VERTEX_SHADER, (char *)vector_shader_src);

  GLuint fragment_shader =
      LoadShader(GL_FRAGMENT_SHADER, (char *)fragment_shader_src);

  glAttachShader(program_object, vertex_shader);
  glAttachShader(program_object, fragment_shader);

  if (!link_program(program_object)) {
    return 0;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  glBindAttribLocation(program_object, 0, "v_position");

  return program_object;
}
