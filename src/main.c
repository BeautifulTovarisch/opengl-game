#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "engine/mod.h"
#include "game/mod.h"
#include "linalg/mod.h"
#include "logger/mod.h"
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

  GLuint program = glCreateProgram();

  if (!Engine_Init(program, WIDTH, HEIGHT)) {
    Log(FATAL, "Engine failed to initialize");
    return 1;
  };

  while (!glfwWindowShouldClose(window)) {

    float frame_start = glfwGetTime();

    Game_ProcessInput(delta_time);
    Game_Update(delta_time, game);
    Game_Render(window);

    delta_time = glfwGetTime() - frame_start;
  }

  Game_Terminate();

  return 0;
}
