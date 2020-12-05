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

GLuint Engine_Init(int width, int height) {
  GLuint prog = glCreateProgram();

  GLuint vertex =
      Shader_Compile(GL_VERTEX_SHADER, "src/shader/glsl/vertex.glsl");

  GLuint fragment =
      Shader_Compile(GL_FRAGMENT_SHADER, "src/shader/glsl/fragment.glsl");

  glAttachShader(prog, vertex);
  glAttachShader(prog, fragment);

  if (!Shader_Link(prog)) {
    return 0;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  // Projection set here due to fixed 2D nature of game.
  Mat4 projection = {0};
  Matrix_Ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f,
               projection);

  glUseProgram(prog);

  Shader_SetInteger(prog, "image", 0);
  Shader_SetMatrix4(prog, "projection", projection);

  Logger_CheckGLErrors("Failed to set shader input.");

  return prog;
}
