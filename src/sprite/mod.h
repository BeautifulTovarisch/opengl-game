#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>

#include "../linalg/mod.h"

void Sprite_Init(GLuint VBO, GLuint VAO);
void Sprite_Draw(GLuint texture, Vector pos, Vector size, Vector color,
                 float rot, GLuint VAO, GLuint prog);

#endif
