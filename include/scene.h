#pragma once

#include "game_state.h"
#include <memory>

struct Scene {
  virtual void enter(GameState& state) {};
  virtual std::unique_ptr<Scene> update(GameState& state, float deltaTime) = 0;
  virtual void draw(const GameState& state) const = 0;
  virtual ~Scene() = default;
};
