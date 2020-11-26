#ifndef GAME_H
#define GAME_H

typedef enum {
  GAME_ACTIVE = 0,
  GAME_MENU = 1 << 0,
  GAME_WIN = 1 << 1
} GameState;

typedef struct Game {
  int keys[1024];
  unsigned int width;
  unsigned int height;
  GameState state;
} Game;

void Game_Update(float dt);
void Game_Render();
void Game_ProcessInput(float dt);

Game *Game_Init(unsigned int width, unsigned int height);

#endif
