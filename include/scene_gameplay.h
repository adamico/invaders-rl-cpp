#pragma once

#include "scene.h"

struct SceneGameplay : Scene {
  void enter(GameState& state) override;
  std::unique_ptr<Scene> update(GameState& state, float dt) override;
  void draw(const GameState& state) const override;
};
