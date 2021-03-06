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
 * poscr -> POsition, Size, Color, Rotation combined in one matrix:
 *
 * |x y 0 0| Position
 * |w h 0 0| Size
 * |r g b 0| Color
 * |1 1 1 Θ| Axis Angle Rotation (in radians)
 *
 */
void Sprite_Draw(GLuint texture, GLuint VAO, GLuint prog, Mat4 poscr) {

  Mat4 model = {0};

  M_Ident(model);

  Vector pos = {poscr[0], poscr[1], poscr[2]};
  Vector size = {poscr[4], poscr[5], poscr[6]};
  Vector color = {poscr[8], poscr[9], poscr[10]};
  Vector axis = {poscr[12], poscr[13], poscr[14]};

  float angle = poscr[15];

  M_Scale(size, model);

  // Rotate about "center" of object
  DualQuat to_center =
      DQ_Pure_Translation((Vector){size.x * 0.5f, size.y * 0.5f, 0.0});

  DualQuat rotate = DQ_Pure_Rotation(Q_From_Axis(axis, angle));

  DualQuat to_orig_pt =
      DQ_Pure_Translation((Vector){-size.x * 0.5f, -size.y * 0.5f, 0.0});

  DualQuat to_pos = DQ_Pure_Translation(pos);

  DualQuat transform =
      DQ_Norm(DQ_Mult(DQ_Mult(DQ_Mult(to_pos, to_center), rotate), to_orig_pt));

  /* DQ_Print(transform); */

  for (int i = 0; i < 16; i++) {
    i % 4 == 0 && printf("\n");
    printf("%f ", model[i]);
  }

  printf("\n");

  Shader_SetMatrix4(prog, "model", model);
  Shader_SetVector3(prog, "sprite_color", color);

  Shader_SetVector4(prog, "rotation", DQ_GetRotation(transform));
  Shader_SetVector4(prog, "translation", DQ_GetTranslation(transform));

  glUseProgram(prog);
  glBindVertexArray(VAO);
}
