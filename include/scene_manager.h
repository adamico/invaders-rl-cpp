#pragma once

#include "scene.h"
#include <memory>

class SceneManager {
public:
  explicit SceneManager(std::unique_ptr<Scene> startScene);
  void tick(GameState& state, float deltaTime);

private:
  std::unique_ptr<Scene> current_;
};
