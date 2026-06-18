#pragma once

#include "raylib.h"
#include <array>

constexpr int MAX_PROJECTILES = 100;
constexpr int MAX_ENEMIES = 55;

extern const Vector2 windowSize;

enum class GameScene { TITLE, GAMEPLAY, GAMEOVER };

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

typedef struct GameState {
  Player player;
  std::array<Projectile, MAX_PROJECTILES> bullets;
  std::array<Enemy, MAX_ENEMIES> enemies;
  Vector2 enemyDirection;
  int enemySpeed;
  int activeEnemies;
  GameScene currentScene;
  bool victory;
  GameResources resources;
  int score;
} GameState;

void CenterText(const char* text, int yPos, int fontSize, Color textColor);
void InitGameplay(GameState* state);
