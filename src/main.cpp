#include "game.h"
#include "raylib.h"
#include "resource_dir.h"
#include "scene_manager.h"
#include "scene_title.h"

#include <stdio.h>

constexpr Vector2 windowSize = {1280, 720};
constexpr char gameName[] = "Invaders RL";

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

  SceneManager scenes(std::make_unique<SceneTitle>());

  while (!WindowShouldClose()) {
    scenes.tick(state, GetFrameTime());
  }

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
