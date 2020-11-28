#include "mod.h"

void Game_Render(GLFWwindow *window) {
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glfwSwapBuffers(window);
};
void Game_Update(float dt){};
// Perform any cleanup necessary
void Game_Terminate() { glfwTerminate(); };

void Game_ProcessInput(float dt) { glfwPollEvents(); };

Game *Game_Init(unsigned int width, unsigned int height) {
  return &(Game){.state = GAME_ACTIVE, .width = width, .height = height};
}
