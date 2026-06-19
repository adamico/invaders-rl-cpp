#pragma once

#include "game.h"
#include "pool.h"
#include <optional>

constexpr int MAX_ENEMIES = 55;

class Swarm {
public:
  void reset();
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void deactivate(Enemy& enemy);

  bool hasBreached() const { return breached; }
  int activeCount() const { return pool.activeCount(); }
  std::optional<Vector2> randomShooterPosition() const;

  auto begin() { return pool.begin(); }
  auto end() { return pool.end(); }
  auto begin() const { return pool.begin(); }
  auto end() const { return pool.end(); }

private:
  Pool<Enemy, MAX_ENEMIES> pool{};

  Vector2 direction{};
  int speed{};
  bool breached{};
};
