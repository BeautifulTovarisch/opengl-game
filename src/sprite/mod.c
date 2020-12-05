#include "mod.h"

GLuint Sprite_Init(GLuint prog) {
  unsigned int VAO, VBO, EBO;

  GLuint indices[6] = {0, 1, 2, 0, 1, 3};

  GLfloat vertices[] = {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  GLint ver_loc = glGetAttribLocation(prog, "vertex");

  glVertexAttribPointer(ver_loc, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (void *)0);
  glEnableVertexAttribArray(ver_loc);

  return VAO;
};

/* Draw sprite given various vectors and a texture.
 * Poscr -> POsition, Size, Color, Rotation combined in one matrix:
 *
 * |x y 0 0| Position
 * |w h 0 0| Size
 * |r g b 0| Color
 * |θ θ θ 0| Rotation (in radians)
 */
void Sprite_Draw(GLuint texture, GLuint VAO, GLuint prog, Mat4 poscr) {

  Mat4 model = {0};

  Matrix_Ident(model);

  Vector pos = {poscr[0], poscr[1], poscr[2]};
  Vector size = {poscr[4], poscr[5], poscr[6]};
  Vector color = {poscr[8], poscr[9], poscr[10]};
  Vector rotation = {poscr[12], poscr[13], poscr[14]};

  // Translate to Sprite position
  Matrix_Trans(pos, model);
  Matrix_Scale(size, model);

  Vector v = Quat_Rot(To_Quat(rotation), pos);

  Shader_SetMatrix4(prog, "model", model);
  Shader_SetVector3(prog, "rotation", v);
  Shader_SetVector3(prog, "sprite_color", color);

  glUseProgram(prog);
  glBindVertexArray(VAO);
}
