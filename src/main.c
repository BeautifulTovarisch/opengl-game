#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/mod.h"
#include "game/mod.h"
#include "linalg/mod.h"
#include "logger/mod.h"
#include "sprite/mod.h"
#include "texture/mod.h"

#define WIDTH 800
#define HEIGHT 600

int main() {
  GLFWwindow *window = Engine_CreateWindow(WIDTH, HEIGHT);

  if (!window) {
    Log(FATAL, "Window failed to initialize. Exiting...");
    return 1;
  }

  Game *game = Game_Init(WIDTH, HEIGHT);

  float delta_time = 0.0f;

  GLuint program = Engine_Init(WIDTH, HEIGHT);

  if (!program) {
    Log(FATAL, "Engine failed to initialize");
    return 1;
  };

  GLuint VAO = Sprite_Init(program);

  Mat4 sprite_attr = {200, 200,  0, 0, 300.0f, 400.0f, 0,          0,
                      0,   1.0f, 0, 0, 0,      0,      To_Rad(45), 0};

  GLuint texture = Texture_Create2D("assets/smiley.png");

  while (!glfwWindowShouldClose(window)) {

    float frame_start = glfwGetTime();

    Game_ProcessInput(delta_time);

    Game_Update(delta_time, game);
    Game_Render(window);

    Sprite_Draw(texture, VAO, program, sprite_attr);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(window);

    delta_time = glfwGetTime() - frame_start;
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(program);

  Game_Terminate();

  return 0;
}
