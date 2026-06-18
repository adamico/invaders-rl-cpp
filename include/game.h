#pragma once

#include "raylib.h"

extern const Vector2 windowSize;

struct Player {
  Vector2 pos{};
  Vector2 dir{};
  float speed{};
  float radius{};
  int health{};

  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
};

struct Projectile {
  Vector2 pos{};
  Vector2 dir{};
  float speed{};
  float radius{};
  bool active{};

  void spawn(Vector2 from, Vector2 dir = {0.0f, -1.0f});
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void deactivate() { active = false; };
};

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
