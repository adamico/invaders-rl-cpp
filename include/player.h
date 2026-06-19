#pragma once
#include <raylib.h>

constexpr float PLAYER_RADIUS = 12.5f;
constexpr float PLAYER_SPEED = 300.0f;
constexpr int PLAYER_LIVES = 3;
constexpr float INVULNERABILITY_TIME = 2.0f;

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
  void die() { lives = 0; };

  bool isVulnerable() const { return invulnerableTimer <= 0.0f; };
  bool isDead() const { return lives <= 0; };
  int livesRemaining() const { return lives; };

private:
  void respawn();
  void loseLife();

  int lives{};
  float invulnerableTimer{};
};
