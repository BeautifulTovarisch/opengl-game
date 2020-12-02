#include "mod.h"

void Sprite_Init(GLuint VBO, GLuint VAO) {
  float vertices[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
                      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f};
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindVertexArray(VAO);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
};

void Sprite_Draw(GLuint texture, Vector pos, Vector size, Vector color,
                 float rot, GLuint VAO, GLuint prog) {
  Mat4 model = {};
  // Translate to Sprite position
  Matrix_Trans(pos, model);
  // Translate rotation to center of sprite (1/2 width,height);
  Matrix_Trans((Vector){.x = size.x / 2, .y = size.y / 2, .z = 0}, model);
  Matrix_Scale(size, model);

  glUniformMatrix4fv(glGetUniformLocation(prog, "model"), 1, 0, model);
  glUniform3f(glGetUniformLocation(prog, "sprite_color"), color.x, color.y,
              color.z);

  glBindTexture(GL_TEXTURE_2D, texture);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);
}
