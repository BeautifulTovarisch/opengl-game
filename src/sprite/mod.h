#ifndef SPRITE_H
#define SPRITE_H

#include <glad/glad.h>

#include "../linalg/mod.h"
#include "../logger/mod.h"
#include "../shader/mod.h"

GLuint Sprite_Init();
void Sprite_Draw(GLuint texture, GLuint VAO, GLuint prog, Mat4 poscr);

#endif
