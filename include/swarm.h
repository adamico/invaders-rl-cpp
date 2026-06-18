#pragma once
#include "raylib.h"
#include <array>

constexpr int MAX_ENEMIES = 55;

struct Enemy {
  Vector2 pos{};
  float radius{};
  int scoreValue{};
  bool active{};

  void moveHorizontally(Vector2 dir, float speed, float deltaTime);
  void moveVertically(float amount);
  void draw(const Texture2D& texture) const;
};

struct Swarm {
  std::array<Enemy, MAX_ENEMIES> enemies{};
  Vector2 direction{};
  int speed{};
  int activeCount{};

  void reset();
  bool update(float deltaTime);
  void draw(const Texture2D& texture) const;
};
