#pragma once

#include "game.h"
#include "scene.h"

struct SceneGameplay : Scene {
  std::unique_ptr<Scene> update(GameState& state, float dt) override;
  void draw(const GameState& state) const override;
};
