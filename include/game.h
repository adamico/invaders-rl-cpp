#pragma once

#include "raylib.h"
#include "swarm.h"
#include <array>

constexpr int MAX_PROJECTILES = 100;

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

typedef struct GameState {
  Player player;
  std::array<Projectile, MAX_PROJECTILES> bullets;
  Swarm swarm;
  bool victory;
  GameResources resources;
  int score;
} GameState;

void CenterText(const char* text, int yPos, int fontSize, Color textColor);
void InitGameplay(GameState* state);
