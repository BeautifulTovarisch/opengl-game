#include "mod.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

GLFWwindow *Engine_CreateWindow(int width, int height) {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, 0);

  GLFWwindow *window = glfwCreateWindow(width, height, "Breakout", NULL, NULL);
  glfwMakeContextCurrent(window);

  if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
    Log(ERROR, "Failed to initialize GLAD");

    return NULL;
  }

  if (window == NULL) {
    Log(ERROR, "Could not create window");
    glfwTerminate();

    return NULL;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glViewport(0, 0, width, height);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  return window;
}

// TODO :: Update Game object state to track program_object
GLint Engine_Init() {
  GLint program_object = glCreateProgram();

  if (!program_object) {
    return 0;
  }

  return program_object;
}
