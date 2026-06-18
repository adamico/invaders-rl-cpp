#include "raylib.h"
#include "raymath.h"
#include <stdarg.h>
#include <stdio.h>

#include "game.h"
#include "scene_gameplay.h"

constexpr int CANVAS_SIZE = 50;
constexpr Vector2 CANVAS_OFFSET = {-CANVAS_SIZE / 2.0f, -CANVAS_SIZE / 2.0f};
constexpr float PLAYER_RADIUS = 12.5f;
constexpr float PLAYER_SPEED = 300.0f;
constexpr int PLAYER_HEALTH = 5;
constexpr int MAX_ENEMIES_PER_ROW = 11;
constexpr int COL_PADDING = 80;
constexpr int ROW_PADDING = 60;
constexpr float ENEMY_SPEED = 50.0f;
constexpr float ENEMY_RADIUS = 15.0f;
constexpr int ENEMY_SCORE_VALUE = 100;
constexpr float PROJECTILE_SPEED = 500.0f;
constexpr float PROJECTILE_RADIUS = 5.0f;

void InitPlayer(GameState* state) {
  Vector2 startPos = {GetScreenWidth() / 2.0f,
                      GetScreenHeight() - (PLAYER_RADIUS * 4)};
  state->player = (Player){.pos = startPos,
                           .radius = PLAYER_RADIUS,
                           .speed = PLAYER_SPEED,
                           .dir = {0.0f, 0.0f},
                           .health = PLAYER_HEALTH};
}

void InitEnemies(GameState* state) {
  Vector2 startGridPos = {
      ((GetScreenWidth() - (MAX_ENEMIES_PER_ROW * COL_PADDING)) / 2.0f) +
          (COL_PADDING / 2.0f),
      100};
  for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES; enemyIndex++) {
    int column = enemyIndex % MAX_ENEMIES_PER_ROW;
    int row = enemyIndex / MAX_ENEMIES_PER_ROW;
    float offsetX = startGridPos.x;
    float offsetY = startGridPos.y;
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

void InitBullets(GameState* state) {
  for (int bulletIndex = 0; bulletIndex < MAX_PROJECTILES; bulletIndex++) {
    state->bullets[bulletIndex] = {.active = false};
  }
}

void InitGameplay(GameState* state) {
  state->score = 0;
  state->victory = false;
  state->activeEnemies = 0;
  InitPlayer(state);
  InitEnemies(state);
  InitBullets(state);
}

void UpdatePlayer(GameState* state, float dt) {
  float radius = state->player.radius;
  float speed = state->player.speed;
  state->player.dir = Vector2Zero();
  Vector2* playerPos = &state->player.pos;
  Vector2* playerDir = &state->player.dir;

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    playerDir->x -= 1.0f;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    playerDir->x += 1.0f;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))
    playerDir->y -= 1.0f;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))
    playerDir->y += 1.0f;

  *playerDir = Vector2Normalize(*playerDir);
  *playerPos = Vector2Add(*playerPos, Vector2Scale(*playerDir, speed * dt));
  *playerPos =
      Vector2Clamp(*playerPos, (Vector2){radius, radius},
                   (Vector2){windowSize.x - radius, windowSize.y - radius});
};

void DrawOffset(Texture2D texture, Vector2 pos, Color tint) {
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, tint);
}

void DrawPlayer(const GameState* state) {
  DrawOffset(state->resources.playerTexture, state->player.pos, WHITE);
  DrawCircleLinesV(state->player.pos, state->player.radius, RED);
};

void PlayerShoot(GameState* state) {
  if (!IsKeyPressed(KEY_SPACE))
    return;

  PlaySound(state->resources.laserSound);
  for (Projectile& bullet : state->bullets) {
    if (bullet.active)
      continue;

    bullet.spawn(state->player.pos);
    break;
  }
}

void Projectile::spawn(Vector2 from, Vector2 dir) {
  pos = from;
  this->dir = dir;
  speed = PROJECTILE_SPEED;
  radius = PROJECTILE_RADIUS;
  active = true;
}

void Projectile::update(float dt) {
  if (!active)
    return;
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  active = pos.y > 0;
}

void Projectile::draw(const Texture2D& texture) const {
  if (!active)
    return;
  DrawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}

void UpdateProjectiles(GameState* state, float dt) {
  for (Projectile& bullet : state->bullets) {
    bullet.update(dt);
  }
}

void DrawProjectiles(const GameState* state) {
  for (const Projectile& bullet : state->bullets) {
    bullet.draw(state->resources.laserTexture);
  }
}

void UpdateEnemies(GameState* state, float dt) {
  Vector2 direction = state->enemyDirection;
  float speed = state->enemySpeed;
  bool needToMoveDown = false;
  int rightEdge = windowSize.x - ENEMY_RADIUS;
  int leftEdge = ENEMY_RADIUS;
  for (Enemy& enemy : state->enemies) {
    if (!enemy.active)
      continue;

    bool willHitRightEdge =
        (enemy.pos.x + enemy.radius) >= rightEdge && direction.x > 0;
    bool willHitLeftEdge =
        (enemy.pos.x - enemy.radius) <= leftEdge && direction.x < 0;
    if (willHitRightEdge || willHitLeftEdge) {
      state->enemyDirection.x *= -1.0f;
      needToMoveDown = true;
      break;
    }
  }

  if (needToMoveDown) {
    for (Enemy& enemy : state->enemies) {
      enemy.pos.y += ROW_PADDING / 2.0;
      if (enemy.pos.y > windowSize.y) {
        state->currentScene = GAMEOVER;
        break;
      }
    }
    needToMoveDown = false;
  }

  for (Enemy& enemy : state->enemies) {
    if (!enemy.active)
      continue;

    Vector2* enemyPos = &enemy.pos;
    *enemyPos = Vector2Add(*enemyPos, Vector2Scale(direction, speed * dt));
  }
}

void DrawEnemies(const GameState* state) {
  for (const Enemy& enemy : state->enemies) {
    if (enemy.active) {
      DrawOffset(state->resources.enemyTexture, enemy.pos, WHITE);
      DrawCircleLinesV(enemy.pos, enemy.radius, RED);
    }
  }
}

void CheckBulletEnemyCollisions(GameState* state) {
  for (Projectile& bullet : state->bullets) {
    if (!bullet.active)
      continue;

    for (Enemy& enemy : state->enemies) {
      if (!enemy.active)
        continue;

      if (CheckCollisionCircles(bullet.pos, bullet.radius, enemy.pos,
                                enemy.radius)) {
        PlaySound(state->resources.explosionSound);
        bullet.active = false;
        enemy.active = false;
        state->activeEnemies--;
        state->score += enemy.scoreValue;
      }
    }
  }
}

void CheckPlayerEnemyCollisions(GameState* state) {
  for (Enemy& enemy : state->enemies) {
    if (!enemy.active)
      continue;

    if (CheckCollisionCircles(state->player.pos, state->player.radius,
                              enemy.pos, enemy.radius)) {
      enemy.active = false;
      state->currentScene = GAMEOVER;
    }
  }
}

void CheckIfPlayerDied(GameState* state) {
  if (state->player.health <= 0)
    state->currentScene = GAMEOVER;
}

void CheckIfPlayerWon(GameState* state) {
  if (state->activeEnemies <= 0) {
    state->victory = true;
    state->currentScene = GAMEOVER;
  }
}

void UpdateGameplay(GameState* state, float dt) {
  CheckIfPlayerDied(state);
  CheckIfPlayerWon(state);
  UpdatePlayer(state, dt);
  UpdateEnemies(state, dt);
  PlayerShoot(state);
  UpdateProjectiles(state, dt);
  CheckBulletEnemyCollisions(state);
  CheckPlayerEnemyCollisions(state);
}

void DrawGameplay(const GameState* state) {
  BeginDrawing();

  // Setup the back buffer for drawing (clear color and depth buffers)
  ClearBackground(BLACK);

  int font_size = 20;

  DrawPlayer(state);
  DrawEnemies(state);
  DrawProjectiles(state);

  DrawText(TextFormat("Health: %i", state->player.health), 20,
           windowSize.y - 40, font_size, WHITE);

  DrawText(TextFormat("Score: %i", state->score), 20, 20, font_size, WHITE);
  EndDrawing();
}
