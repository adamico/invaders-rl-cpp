#pragma once
#include <raylib.h>

constexpr float PLAYER_RADIUS = 12.5f;
constexpr float PLAYER_SPEED = 300.0f;
constexpr int PLAYER_HEALTH = 5;

class Player {
public:
  Vector2 pos{};
  Vector2 dir{};
  float speed{};
  float radius{};

  void reset();
  void update(float deltaTime);
  void draw(const Texture2D& texture) const;
  void takeDamage(int amount = 1);
  void die();

  bool isAlive() const { return hp > 0; };
  int health() const { return hp; };

private:
  int hp{};
};
