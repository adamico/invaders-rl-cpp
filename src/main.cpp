#include "game_state.h"
#include "raylib.h"
#include "resource_dir.h"
#include "scene_manager.h"
#include "scene_title.h"
#include <stdio.h>

constexpr char gameName[] = "Invaders RL";

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
  initGameplay(&state);

  SceneManager scenes(std::make_unique<SceneTitle>());

  while (!WindowShouldClose()) {
    scenes.tick(state, GetFrameTime());
  }

  CloseAudioDevice();
  CloseWindow();
  return 0;
}
