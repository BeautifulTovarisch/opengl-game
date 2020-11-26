#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/mod.h"
#include "logger/mod.h"
#include "texture/mod.h"

void init_buffers(GLuint *VBO, GLuint *VAO, GLuint *EBO,
                  GLuint program_object) {
  GLuint indices[6] = {0, 1, 2, 0, 2, 3};

  GLfloat vertices[] = {// Position 0
                        -0.5f, 0.5f, 0.0f,
                        // Texture Coordinates 0
                        0.0f, 0.0f,
                        // Position 1
                        -0.5f, -0.5f, 0.0f,
                        // Texture Coordinates 1
                        0.0f, 1.0f,
                        // Position 2
                        0.5f, -0.5f, 0.0f,
                        // Texture Coordinates 2
                        1.0f, 1.0f,
                        // Position 3
                        0.5f, 0.5f, 0.0f,
                        // Texture Coordinates 3
                        1.0f, 0.0f};

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
  GLint tex_location = glGetAttribLocation(program_object, "a_tex_coord");

  // Stride length 5 here to capture position and texture coordinates
  glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), (void *)0);

  glVertexAttribPointer(tex_location, 2, GL_FLOAT, GL_FALSE,
                        5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));

  Logger_CheckGLErrors("Error assigning attribute pointers");

  glEnableVertexAttribArray(pos_location);
  glEnableVertexAttribArray(tex_location);
}

int main() {
  Log(INFO, "hey");
  /* GLFWwindow *window = Engine_CreateWindow(1024, 720); */

  /* if (!window) { */
  /*   return 0; */
  /* } */

  /* GLint program_object = Engine_Init(); */

  /* if (!program_object) { */
  /*   return 0; */
  /* } */

  /* int width, height; */
  /* char *data = Texture_Load("assets/lenna.png", &width, &height); */

  /* if (data == NULL) { */
  /*   fprintf(stderr, "Failed to load asset\n"); */
  /*   return 0; */
  /* } */

  /* GLuint texture_id = Texture_Create2D(width, height, data); */

  /* while (!glfwWindowShouldClose(window)) { */
  /*   process_input(window); */
  /*   glClearColor(0.2f, 0.3f, 0.3f, 1.0f); */
  /*   glClear(GL_COLOR_BUFFER_BIT); */

  /*   glUseProgram(program_object); */
  /*   glBindVertexArray(VAO); */
  /*   glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); */

  /*   glfwSwapBuffers(window); */
  /*   glfwPollEvents(); */
  /* } */

  /* glDeleteProgram(program_object); */

  /* glfwTerminate(); */
  /* glDeleteTextures(1, &texture_id); */

  /* Texture_Cleanup(data); */
  return 1;
}
