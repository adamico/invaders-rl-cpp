#pragma once
#include "game.h"
#include "pool.h"

constexpr int MAX_ENEMIES = 55;

class Swarm {
public:
  void reset();
  bool update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void deactivate(Enemy& enemy);
  int activeCount() const { return pool.activeCount(); }

  auto begin() { return pool.begin(); }
  auto end() { return pool.end(); }
  auto begin() const { return pool.begin(); }
  auto end() const { return pool.end(); }

private:
  Pool<Enemy, MAX_ENEMIES> pool{};

  Vector2 direction{};
  int speed{};
};
