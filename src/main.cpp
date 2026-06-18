#include "game.h"
#include "raylib.h"
#include "resource_dir.h"
#include "scene.h"
#include <memory>

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
  state.currentScene = GameScene::TITLE;

  std::unique_ptr<Scene> current = makeScene(GameScene::TITLE);
  GameScene shown = GameScene::TITLE;

  while (!WindowShouldClose()) {
    if (state.currentScene != shown) {
      current = makeScene(state.currentScene);
      shown = state.currentScene;
    }
    current->update(state, GetFrameTime());
    current->draw(state);
  }

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
