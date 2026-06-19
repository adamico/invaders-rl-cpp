#pragma once

#include "raylib.h"

extern const Vector2 windowSize;

struct Enemy {
  Vector2 pos{};
  float radius{};
  int scoreValue{};
  bool active{};

  void moveHorizontally(Vector2 dir, float speed, float deltaTime);
  void moveVertically(float amount);
  void draw(const Texture2D& texture) const;
};

struct GameResources {
  Texture2D playerTexture;
  Texture2D enemyTexture;
  Texture2D laserTexture;
  Sound laserSound;
  Sound explosionSound;

  GameResources();
  ~GameResources();

  GameResources(const GameResources&) = delete;
  GameResources& operator=(const GameResources&) = delete;
};
