#pragma once

#include "game_state.h"
#include "scene.h"
#include <memory>

class SceneManager {
public:
  SceneManager(std::unique_ptr<Scene> startScene, GameState& state);
  void tick(GameState& state, float deltaTime);

private:
  void switchTo(std::unique_ptr<Scene> nextScene, GameState& state);
  std::unique_ptr<Scene> current_;
};
