#pragma once

#include "raylib.h"

extern const Vector2 windowSize;

class Player {
public:
  Vector2 pos{};
  Vector2 dir{};
  float speed{};
  float radius{};

  void reset(Vector2 startPosition);
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void takeDamage(int amount = 1);
  void die();

  bool isAlive() const { return hp > 0; };
  int health() const { return hp; };

private:
  int hp{};
};

struct ProjectileSpec {
  float radius = 5;
  float speed = 500.0f;
  Vector2 dir = {0.0f, -1.0f};
  bool flipVertical = false;
};

struct Projectile {
  Vector2 pos{};
  Vector2 dir{};
  float speed{};
  float radius{};
  bool flipVertical{};
  bool active{};

  void spawn(Vector2 from, const ProjectileSpec& spec);
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
