#ifndef GAME_H
#define GAME_H

#include "raylib.h"
#include "player.hpp"

#define CANVAS_SIZE 50

#define CANVAS_OFFSET (Vector2){-CANVAS_SIZE / 2.0, -CANVAS_SIZE / 2.0}

#define MAX_PROJECTILES 100
#define MAX_ENEMIES 55

#define FOR_EACH_PROJECTILE(projectilePtr, projectileArray)                    \
  for (Projectile *projectilePtr = projectileArray;                            \
       projectilePtr < projectileArray + MAX_PROJECTILES; projectilePtr++)

extern const Vector2 windowSize;

typedef enum GameScene { TITLE, GAMEPLAY, GAMEOVER } GameScene;


// Bullet
typedef struct Projectile {
  Vector2 pos;
  Vector2 dir;
  float speed;
  float radius;
  bool active;
} Projectile;

// Enemies
typedef struct Enemy {
  Vector2 pos;
  float radius;
  bool active;
  int scoreValue;
} Enemy;

typedef struct GameResources {
  Texture2D playerTexture;
  Texture2D enemyTexture;
  Texture2D laserTexture;
  Sound laserSound;
  Sound explosionSound;
} GameResources;

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

void LoadGameResources(GameResources *resources);
void UnloadGameResources(GameResources *resources);

void CenterText(const char *text, int yPos, int fontSize, Color textColor);
void InitGameplay(GameState *state);
void DrawOffset(Texture2D texture, Vector2 pos, Color tint);

#endif
