#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/mod.h"
#include "game/mod.h"
#include "linalg/mod.h"
#include "logger/mod.h"
#include "sprite/mod.h"
#include "texture/mod.h"

#define WIDTH 1024
#define HEIGHT 720

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

  Mat4 sprite_attr = {200.0f, 200.0f, 0, 0, 300.0f, 400.0f, 0, 0,
                      0,      1.0f,   0, 0, 0,      0,      0, 0};

  GLuint texture = Texture_Create2D("assets/lenna.png");

  while (!glfwWindowShouldClose(window)) {

    float frame_start = glfwGetTime();

    Game_ProcessInput(delta_time);

    Game_Update(delta_time, game);
    Game_Render(window);

    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
    glfwPollEvents();
    /* Sprite_Draw(texture, VAO, program, sprite_attr); */

    delta_time = glfwGetTime() - frame_start;
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteProgram(program);

  Game_Terminate();

  return 0;
}
