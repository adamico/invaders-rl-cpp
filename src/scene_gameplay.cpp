#include "raylib.h"
#include "raymath.h"
#include <stdarg.h>
#include <stdio.h>

#include "hud.h"
#include "projectile_pool.h"
#include "scene_gameover.h"
#include "scene_gameplay.h"

constexpr int CANVAS_SIZE = 50;
constexpr Vector2 CANVAS_OFFSET = {-CANVAS_SIZE / 2.0f, -CANVAS_SIZE / 2.0f};
constexpr float PLAYER_RADIUS = 12.5f;
constexpr float PLAYER_SPEED = 300.0f;
constexpr int PLAYER_HEALTH = 5;

void InitPlayer(GameState* state) {
  Vector2 startPos = {GetScreenWidth() / 2.0f,
                      GetScreenHeight() - (PLAYER_RADIUS * 4)};
  state->player = (Player){.pos = startPos,
                           .dir = {0.0f, 0.0f},
                           .speed = PLAYER_SPEED,
                           .radius = PLAYER_RADIUS,
                           .health = PLAYER_HEALTH};
}

void InitGameplay(GameState* state) {
  state->score = 0;
  state->victory = false;
  InitPlayer(state);
  state->swarm.reset();
  state->projectilePool.reset();
}

void Player::update(float dt) {
  dir = Vector2Zero();

  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) dir.x -= 1.0f;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) dir.x += 1.0f;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) dir.y -= 1.0f;
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) dir.y += 1.0f;

  dir = Vector2Normalize(dir);
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  pos = Vector2Clamp(
      pos, (Vector2){radius, radius},
      (Vector2){GetScreenWidth() - radius, GetScreenHeight() - radius});
};

void DrawOffset(Texture2D texture, Vector2 pos, Color tint) {
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, tint);
}

void Player::draw(const Texture2D& texture) const {
  DrawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
};

void PlayerShoot(GameState& state) {
  if (!IsKeyPressed(KEY_SPACE)) return;

  if (state.projectilePool.fire(state.player.pos, {0.0f, -1.0f}))
    PlaySound(state.resources.laserSound);
}

void Projectile::update(float dt) {
  if (!active) return;
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  active = pos.y > 0;
}

void Projectile::draw(const Texture2D& texture) const {
  if (!active) return;
  DrawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}

void Enemy::moveHorizontally(Vector2 dir, float speed, float deltaTime) {
  pos = Vector2Add(pos, Vector2Scale(dir, speed * deltaTime));
}

void Enemy::moveVertically(float amount) { pos.y += amount; }

void Enemy::draw(const Texture2D& texture) const {
  if (!active) return;
  DrawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}

void CheckBulletEnemyCollisions(GameState& state) {
  for (Projectile& bullet : state.projectilePool.projectiles) {
    if (!bullet.active) continue;

    for (Enemy& enemy : state.swarm.enemies) {
      if (!enemy.active) continue;

      if (CheckCollisionCircles(bullet.pos, bullet.radius, enemy.pos,
                                enemy.radius)) {
        PlaySound(state.resources.explosionSound);
        bullet.deactivate();
        state.swarm.deactivate(enemy);
        state.score += enemy.scoreValue;
      }
    }
  }
}

void CheckPlayerEnemyCollisions(GameState& state) {
  for (Enemy& enemy : state.swarm.enemies) {
    if (!enemy.active) continue;

    if (CheckCollisionCircles(state.player.pos, state.player.radius, enemy.pos,
                              enemy.radius)) {
      state.swarm.deactivate(enemy);
      state.player.health = 0;
    }
  }
}

std::unique_ptr<Scene> SceneGameplay::update(GameState& state, float dt) {
  if (state.swarm.update(dt)) state.player.health = 0;
  state.player.update(dt);
  PlayerShoot(state);
  state.projectilePool.update(dt);
  CheckBulletEnemyCollisions(state);
  CheckPlayerEnemyCollisions(state);

  if (state.swarm.activeCount <= 0) state.victory = true;
  if (state.victory || state.player.health <= 0)
    return std::make_unique<SceneGameover>();

  return nullptr;
}

void SceneGameplay::draw(const GameState& state) const {
  BeginDrawing();

  ClearBackground(BLACK);
  state.player.draw(state.resources.playerTexture);
  state.swarm.draw(state.resources.enemyTexture);
  state.projectilePool.draw(state.resources.laserTexture);
  drawHud(state.player.health, state.score);

  EndDrawing();
}
