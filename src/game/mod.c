#include "mod.h"

void Game_Update(float dt){};
void Game_Render(){};
void Game_ProcessInput(float dt){};

Game *Game_Init(unsigned int width, unsigned int height) {
  return &(Game){.state = GAME_ACTIVE,
                   .width = width,
                   .height = height};
}
