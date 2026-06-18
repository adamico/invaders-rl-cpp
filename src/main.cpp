#include "raylib.h"
#include "resource_dir.h"
#include "scene_gameover.h"
#include "scene_gameplay.h"
#include "scene_title.h"
#include <stdio.h>

const Vector2 windowSize = {1280, 720};
const char *gameName = "Invaders RL";

GameResources::GameResources() {
  playerTexture = LoadTexture("player.png");
  enemyTexture = LoadTexture("enemy.png");
  laserTexture = LoadTexture("laser.png");
  laserSound = LoadSound("laser.wav");
  explosionSound = LoadSound("explosion.wav");
}

GameResources::~GameResources() {
  UnloadTexture(playerTexture);
  UnloadTexture(enemyTexture);
  UnloadTexture(laserTexture);
  UnloadSound(laserSound);
  UnloadSound(explosionSound);
}

// Main function
int main() {
#ifdef DEBUG
  printf(">>> ENGINE BOOTING IN DEBUG MODE\n");
#else
  printf(">>> ENGINE BOOTING IN RELEASE MODE\n");
#endif

  SearchAndSetResourceDir("resources");
  SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

  InitWindow(windowSize.x, windowSize.y, gameName);
  InitAudioDevice();

  GameState state = {};

  InitGameplay(&state);

  state.currentScene = TITLE;

  while (!WindowShouldClose()) {
    switch (state.currentScene) {
    case TITLE:
      UpdateTitle(&state);
      DrawTitle(&state);
      break;
    case GAMEPLAY:
      UpdateGameplay(&state, GetFrameTime());
      DrawGameplay(&state);
      break;
    case GAMEOVER:
      UpdateGameover(&state);
      DrawGameover(&state);
      break;
    }
  }

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
