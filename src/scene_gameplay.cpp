#include "collision.h"
#include "raylib.h"
#include "raymath.h"
#include <stdarg.h>
#include <stdio.h>

#include "collision.h"
#include "hud.h"
#include "projectile_pool.h"
#include "scene_gameover.h"
#include "scene_gameplay.h"

constexpr int CANVAS_SIZE = 50;
constexpr Vector2 CANVAS_OFFSET = {-CANVAS_SIZE / 2.0f, -CANVAS_SIZE / 2.0f};
constexpr float PLAYER_RADIUS = 12.5f;
constexpr float PLAYER_SPEED = 300.0f;
constexpr int PLAYER_HEALTH = 5;

constexpr float ENEMY_FIRE_COOLDOWN = 0.8f;
constexpr float ENEMY_PROJECTILE_SPEED = 300.0f;

constexpr float PROJECTILE_RADIUS = 5.0f;

void initPlayer(GameState* state) {
  Vector2 startPos = {GetScreenWidth() / 2.0f,
                      GetScreenHeight() - (PLAYER_RADIUS * 4)};
  state->player = (Player){.pos = startPos,
                           .dir = {0.0f, 0.0f},
                           .speed = PLAYER_SPEED,
                           .radius = PLAYER_RADIUS,
                           .health = PLAYER_HEALTH};
}

void initGameplay(GameState* state) {
  state->score = 0;
  state->victory = false;
  initPlayer(state);
  state->swarm.reset();
  state->projectilePool.reset();
  state->enemyProjectilePool.reset();
  state->enemyFireCooldown = ENEMY_FIRE_COOLDOWN;
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

void drawOffset(Texture2D texture, Vector2 pos, Color tint) {
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, tint);
}

void Player::draw(const Texture2D& texture) const {
  drawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
};

void playerShoot(GameState& state) {
  if (!IsKeyPressed(KEY_SPACE)) return;

  if (state.projectilePool.fire(state.player.pos, {0.0f, -1.0f}))
    PlaySound(state.resources.laserSound);
}

void Projectile::spawn(Vector2 from, Vector2 dir, float speed) {
  pos = from;
  this->dir = dir;
  this->speed = speed;
  radius = PROJECTILE_RADIUS;
  active = true;
}

void Projectile::update(float dt) {
  if (!active) return;
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  active = pos.y > 0;
}

void Projectile::draw(const Texture2D& texture) const {
  if (!active) return;
  drawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}

void updateEnemyFire(GameState& state, float dt) {
  state.enemyFireCooldown -= dt;
  if (state.enemyFireCooldown > 0.0f) return;

  Enemy* shooter = nullptr;

  int seen = 0;
  for (Enemy& enemy : state.swarm) {
    if (!enemy.active) continue;
    ++seen;
    if (GetRandomValue(1, seen) == 1) shooter = &enemy;
  }

  if (shooter) {
    state.enemyProjectilePool.fire(shooter->pos, {0.0f, 1.0f},
                                   ENEMY_PROJECTILE_SPEED);
  }
  state.enemyFireCooldown = ENEMY_FIRE_COOLDOWN;
}

void Enemy::moveHorizontally(Vector2 dir, float speed, float deltaTime) {
  pos = Vector2Add(pos, Vector2Scale(dir, speed * deltaTime));
}

void Enemy::moveVertically(float amount) { pos.y += amount; }

void Enemy::draw(const Texture2D& texture) const {
  if (!active) return;
  drawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
}

void detectBulletEnemyCollisions(GameState& state) {
  for (Projectile& bullet : state.projectilePool) {
    if (!bullet.active) continue;

    for (Enemy& enemy : state.swarm) {
      if (!enemy.active) continue;

      if (overlaps(bullet, enemy)) {
        PlaySound(state.resources.explosionSound);
        bullet.deactivate();
        state.swarm.deactivate(enemy);
        // REFACTOR: should be state.score.add(enemy.scoreValue);
        state.score += enemy.scoreValue;
      }
    }
  }
}

void detectPlayerEnemyCollisions(GameState& state) {
  for (Enemy& enemy : state.swarm) {
    if (!enemy.active) continue;

    if (overlaps(state.player, enemy)) {
      state.swarm.deactivate(enemy);
      // REFACTOR: should be state.player.die();
      state.player.health = 0;
    }
  }
}

void detectEnemyBulletPlayerCollisions(GameState& state) {
  for (Projectile& bullet : state.enemyProjectilePool) {
    if (!bullet.active) continue;

    if (overlaps(state.player, bullet)) {
      bullet.deactivate();
      // REFACTOR: should be state.player.takeDamage();
      state.player.health -= 1;
    }
  }
}

std::unique_ptr<Scene> SceneGameplay::update(GameState& state, float dt) {
  // REFACTOR: the return value of swarm.update() is unclear
  // REFACTOR: should be state.player.die();
  if (state.swarm.update(dt)) state.player.health = 0;
  state.player.update(dt);
  playerShoot(state);
  updateEnemyFire(state, dt);
  state.projectilePool.update(dt);
  state.enemyProjectilePool.update(dt);
  detectBulletEnemyCollisions(state);
  detectPlayerEnemyCollisions(state);
  detectEnemyBulletPlayerCollisions(state);

  if (state.swarm.activeCount() <= 0) state.victory = true;
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
  state.enemyProjectilePool.draw(state.resources.laserTexture);
  drawHud(state.player.health, state.score);

  EndDrawing();
}
