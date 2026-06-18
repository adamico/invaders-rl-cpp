#pragma once
#include "game.h"
#include "pool.h"

constexpr int MAX_ENEMIES = 55;

struct Swarm {
  Pool<Enemy, MAX_ENEMIES> pool{};

  Vector2 direction{};
  int speed{};

  void reset();
  bool update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void deactivate(Enemy& enemy);
  int activeCount() const { return pool.activeCount(); }
};
