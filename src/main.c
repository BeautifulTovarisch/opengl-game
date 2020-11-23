#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glad/glad.h>

#define GLFW_INCLUDE_ES2
#include <GLFW/glfw3.h>

#include "shader/mod.h"

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
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

GLint Init() {
  GLbyte vector_shader_src[] =
      "#version 330 core\n"
      "layout (location = 0) in vec3 aPos;\n"
      "void main()\n"
      "{\n"
      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
      "}\0";

  GLbyte fragment_shader_src[] =
      "#version 330 core\n"
      "out vec4 FragColor;\n"
      "void main()\n"
      "{\n"
      "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

int main() {
  GLuint indices[] = {0, 1, 3};

  GLfloat vertices[] = {0.0f, 0.5f, 0.0f,  -0.5f, -0.5f,
                        0.0f, 0.5f, -0.5f, 0.0f};

  GLFWwindow *window = init_window(1024, 720);

  if (!window) {
    return 0;
  }

  GLint program_object = Init();

  if (!program_object) {
    return 0;
  }

  GLuint VBO, VAO, EBO;

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(program_object);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(program_object);

  glfwTerminate();
  return 1;
}
