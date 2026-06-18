#pragma once
#include "game.h"
#include <array>

constexpr int MAX_ENEMIES = 55;

struct Swarm {
  std::array<Enemy, MAX_ENEMIES> enemies{};
  Vector2 direction{};
  int speed{};
  int activeCount{};

  void reset();
  bool update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void deactivate(Enemy& enemy);
};
