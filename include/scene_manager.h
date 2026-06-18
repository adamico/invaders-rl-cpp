#pragma once

#include "scene.h"
#include <memory>

class SceneManager {
public:
  explicit SceneManager(GameScene startScene);
  void tick(GameState& state, float deltaTime);

private:
  std::unique_ptr<Scene> current_;
  GameScene shown_;
};
