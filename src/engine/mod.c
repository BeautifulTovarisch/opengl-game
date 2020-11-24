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
    Logger_GetLogInfo("Error linking program", program_object, 0);
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
  GLint program_object = glCreateProgram();

  if (!program_object) {
    return 0;
  }

  // Compile and attach shaders
  GLuint vertex_shader = LoadShader(GL_VERTEX_SHADER, "src/shader/vertex.glsl");

  GLuint fragment_shader =
      LoadShader(GL_FRAGMENT_SHADER, "src/shader/fragment.glsl");

  glAttachShader(program_object, vertex_shader);
  glAttachShader(program_object, fragment_shader);

  if (!link_program(program_object)) {
    return 0;
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);

  return program_object;
}
