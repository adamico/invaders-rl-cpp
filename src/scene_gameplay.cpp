#include "raylib.h"
#include "raymath.h"
#include <stdarg.h>
#include <stdio.h>

#include "game.h"
#include "scene_gameplay.h"

#define MAX_ENEMIES_PER_ROW 11
#define COL_PADDING 80
#define ROW_PADDING 60
#define ENEMY_SPEED 50.0f
#define ENEMY_RADIUS 15.0f
#define ENEMY_SCORE_VALUE 100
#define START_GRID_POS                                                         \
  (Vector2){((windowSize.x - (MAX_ENEMIES_PER_ROW * COL_PADDING)) / 2) +       \
                (COL_PADDING / 2.0f),                                          \
            100}

#define FOR_EACH_ENEMY(enemyPtr, enemyArray)                                   \
  for (Enemy *enemyPtr = enemyArray; enemyPtr < enemyArray + MAX_ENEMIES;      \
       enemyPtr++)


void InitEnemies(GameState *state) {
  for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES; enemyIndex++) {
    int column = enemyIndex % MAX_ENEMIES_PER_ROW;
    int row = enemyIndex / MAX_ENEMIES_PER_ROW;
    float offsetX = START_GRID_POS.x;
    float offsetY = START_GRID_POS.y;
    Enemy enemy = {.pos = (Vector2){offsetX + (column * COL_PADDING),
                                    offsetY + (row * ROW_PADDING)},
                   .radius = ENEMY_RADIUS,
                   .active = true,
                   .scoreValue = ENEMY_SCORE_VALUE};
    state->enemies[enemyIndex] = enemy;
    state->enemyDirection = (Vector2){1.0f, 0.0f};
    state->enemySpeed = ENEMY_SPEED;
    state->activeEnemies++;
  }
}

void InitGameplay(GameState *state) {
  GameResources resBackup = state->resources;
  *state = (GameState){};
  state->resources = resBackup;

  state->victory = false;
  InitEnemies(state);
}


void DrawOffset(Texture2D texture, Vector2 pos, Color tint) {
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, tint);
}


void UpdateProjectiles(GameState *state, float dt) {
  FOR_EACH_PROJECTILE(bullet, state->bullets) {
    if (!bullet->active)
      continue;
    bullet->pos =
        Vector2Add(bullet->pos, Vector2Scale(bullet->dir, bullet->speed * dt));
    bullet->active = bullet->pos.y > 0;
  }
}

void DrawProjectiles(GameState *state) {
  FOR_EACH_PROJECTILE(bullet, state->bullets) {
    if (bullet->active) {
      DrawOffset(state->resources.laserTexture, bullet->pos, WHITE);
      DrawCircleLinesV(bullet->pos, bullet->radius, RED);
    }
  }
}

void UpdateEnemies(GameState *state, float dt) {
  Vector2 direction = state->enemyDirection;
  int speed = state->enemySpeed;
  bool needToMoveDown = false;
  int rightEdge = windowSize.x - ENEMY_RADIUS;
  int leftEdge = ENEMY_RADIUS;
  FOR_EACH_ENEMY(enemy, state->enemies) {
    if (!enemy->active)
      continue;

    bool willHitRightEdge =
        (enemy->pos.x + enemy->radius) >= rightEdge && direction.x > 0;
    bool willHitLeftEdge =
        (enemy->pos.x - enemy->radius) <= leftEdge && direction.x < 0;
    if (willHitRightEdge || willHitLeftEdge) {
      state->enemyDirection.x *= -1.0f;
      needToMoveDown = true;
      break;
    }
  }

  if (needToMoveDown) {
    FOR_EACH_ENEMY(enemy, state->enemies) {
      enemy->pos.y += ROW_PADDING / 2.0;
      if (enemy->pos.y > windowSize.y) {
        state->currentScene = GAMEOVER;
        break;
      }
    }
    needToMoveDown = false;
  }

  FOR_EACH_ENEMY(enemy, state->enemies) {
    if (!enemy->active)
      continue;

    Vector2 *enemyPos = &enemy->pos;
    *enemyPos = Vector2Add(*enemyPos, Vector2Scale(direction, speed * dt));
  }
}

void DrawEnemies(GameState *state) {
  FOR_EACH_ENEMY(enemy, state->enemies) {
    if (enemy->active) {
      DrawOffset(state->resources.enemyTexture, enemy->pos, WHITE);
      DrawCircleLinesV(enemy->pos, enemy->radius, RED);
    }
  }
}

void CheckBulletEnemyCollisions(GameState *state) {
  FOR_EACH_PROJECTILE(bullet, state->bullets) {
    if (!bullet->active)
      continue;

    FOR_EACH_ENEMY(enemy, state->enemies) {
      if (!enemy->active)
        continue;

      if (CheckCollisionCircles(bullet->pos, bullet->radius, enemy->pos,
                                enemy->radius)) {
        PlaySound(state->resources.explosionSound);
        bullet->active = false;
        enemy->active = false;
        state->activeEnemies--;
        state->score += enemy->scoreValue;
      }
    }
  }
}

void CheckPlayerEnemyCollisions(GameState *state) {
  FOR_EACH_ENEMY(enemy, state->enemies) {
    if (!enemy->active)
      continue;

    if (CheckCollisionCircles(state->player.pos, state->player.radius,
                              enemy->pos, enemy->radius)) {
      enemy->active = false;
      state->currentScene = GAMEOVER;
    }
  }
}

void CheckIfPlayerDied(GameState *state) {
  if (state->player.health <= 0)
    state->currentScene = GAMEOVER;
}

void CheckIfPlayerWon(GameState *state) {
  if (state->activeEnemies <= 0) {
    state->victory = true;
    state->currentScene = GAMEOVER;
  }
}

void UpdateGameplay(GameState* state, float dt) {
  CheckIfPlayerDied(state);
  CheckIfPlayerWon(state);
  state->player.Update(state, dt);
  UpdateEnemies(state, dt);
  state->player.Shoot(state);
  UpdateProjectiles(state, dt);
  CheckBulletEnemyCollisions(state);
  CheckPlayerEnemyCollisions(state);
}

void DrawGameplay(GameState* state) {
  BeginDrawing();

  // Setup the back buffer for drawing (clear color and depth buffers)
  ClearBackground(BLACK);

  int font_size = 20;

  state->player.Draw(state);
  DrawEnemies(state);
  DrawProjectiles(state);

  DrawText(TextFormat("Health: %i", state->player.health), 20,
           windowSize.y - 40, font_size, WHITE);

  DrawText(TextFormat("Score: %i", state->score), 20, 20, font_size, WHITE);
  EndDrawing();
}
