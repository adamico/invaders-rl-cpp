#ifndef GAME_H
#define GAME_H

#include "raylib.h"

constexpr int MAX_PROJECTILES = 100;
constexpr int MAX_ENEMIES = 55;

extern const Vector2 windowSize;

typedef enum GameScene { TITLE, GAMEPLAY, GAMEOVER } GameScene;

typedef struct Player {
  Vector2 pos;
  float radius;
  float speed;
  Vector2 dir;
  int health;
} Player;

typedef struct Projectile {
  Vector2 pos;
  Vector2 dir;
  float speed;
  float radius;
  bool active;
} Projectile;

typedef struct Enemy {
  Vector2 pos;
  float radius;
  bool active;
  int scoreValue;
} Enemy;

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
  Projectile bullets[MAX_PROJECTILES];
  Enemy enemies[MAX_ENEMIES];
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

#endif
