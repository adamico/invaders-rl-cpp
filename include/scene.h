#pragma once

#include "game.h"
#include <memory>

struct Scene {
  virtual void update(GameState& state, float deltaTime) = 0;
  virtual void draw(const GameState& state) const = 0;
  virtual ~Scene() = default;
};

std::unique_ptr<Scene> makeScene(GameScene which);
