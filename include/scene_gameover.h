#pragma once

#include "scene.h"

struct SceneGameover : Scene {
  std::unique_ptr<Scene> update(GameState& state, float deltaTime) override;
  void draw(const GameState& state) const override;
};
