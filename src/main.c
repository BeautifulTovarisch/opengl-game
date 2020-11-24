#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/mod.h"
#include "image_loader/mod.h"

void init_buffers(GLuint *VBO, GLuint *VAO, GLuint *EBO,
                  GLuint program_object) {
  GLuint indices[6] = {0, 1, 2, 0, 2, 3};

  GLfloat vertices[] = {-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,  -0.5f, -0.5f,
                        0.0f,  0.0f, 1.0f, 0.5f, -0.5f, 0.0f,  1.0f,
                        1.0f,  0.5f, 0.5f, 0.0f, 1.0f,  0.0f};

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glGenBuffers(1, EBO);

  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  GLint pos_location = glGetAttribLocation(program_object, "a_pos");

  glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)0);
  glEnableVertexAttribArray(pos_location);

  glBindBuffer(GL_ARRAY_BUFFER, pos_location);
  glBindVertexArray(0);
}

int main() {
  GLFWwindow *window = init_window(1024, 720);

  if (!window) {
    return 0;
  }

  GLint program_object = Engine_Init();

  if (!program_object) {
    return 0;
  }

  GLuint VBO, VAO, EBO;

  init_buffers(&VBO, &VAO, &EBO, program_object);

  int width, height;
  char *data = IL_Load("assets/lenna.png", &width, &height);

  if (data == NULL) {
    fprintf(stderr, "Failed to load asset\n");
    return 0;
  }

  GLuint texture_id = IL_CreateTexture2D(width, height, data);

  glBindTexture(GL_TEXTURE_2D, texture_id);

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

  IL_Cleanup(data);
  return 1;
}
