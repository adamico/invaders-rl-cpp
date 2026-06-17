#ifndef GAME_H
#define GAME_H

#include "raylib.h"

#include "player.hpp"
#include "projectile.hpp"
#include "enemy.hpp"

#define CANVAS_SIZE 50

#define CANVAS_OFFSET (Vector2){-CANVAS_SIZE / 2.0, -CANVAS_SIZE / 2.0}

#define MAX_PROJECTILES 100
#define MAX_ENEMIES 55

#define FOR_EACH_PROJECTILE(projectilePtr, projectileArray)                    \
  for (Projectile *projectilePtr = projectileArray;                            \
       projectilePtr < projectileArray + MAX_PROJECTILES; projectilePtr++)

extern const Vector2 windowSize;

typedef enum GameScene { TITLE, GAMEPLAY, GAMEOVER } GameScene;


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
  bool needToMoveDown;
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
