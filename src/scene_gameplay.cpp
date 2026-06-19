#include "collision.h"
#include "game.h"
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
constexpr float PLAYER_PROJECTILE_RADIUS = 5.0f;
constexpr float PLAYER_PROJECTILE_SPEED = 500.0f;
constexpr float ENEMY_FIRE_COOLDOWN = 0.8f;
constexpr float ENEMY_PROJECTILE_RADIUS = 5.0f;
constexpr float ENEMY_PROJECTILE_SPEED = 350.0f;

constexpr ProjectileSpec PLAYER_PROJECTILE_SPEC = {
    .radius = PLAYER_PROJECTILE_RADIUS,
    .speed = PLAYER_PROJECTILE_SPEED,
    .dir = {0.0f, -1.0f},
    .flipVertical = false,
};

constexpr ProjectileSpec ENEMY_PROJECTILE_SPEC = {
    .radius = ENEMY_PROJECTILE_RADIUS,
    .speed = ENEMY_PROJECTILE_SPEED,
    .dir = {0.0f, 1.0f},
    .flipVertical = true,
};

void drawOffset(Texture2D texture, Vector2 pos, Color tint) {
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, tint);
}

void Player::reset(Vector2 startPosition) {
  pos = startPosition;
  dir = {0.0f, 0.0f};
  speed = PLAYER_SPEED;
  radius = PLAYER_RADIUS;
  hp = PLAYER_HEALTH;
}

void initGameplay(GameState* state) {
  state->score = 0;
  state->victory = false;
  Vector2 playerStartPosition = {GetScreenWidth() / 2.0f,
                                 GetScreenHeight() - (PLAYER_RADIUS * 4)};
  state->player.reset(playerStartPosition);
  state->swarm.reset();
  state->projectilePool.reset(PLAYER_PROJECTILE_SPEC);
  state->enemyProjectilePool.reset(ENEMY_PROJECTILE_SPEC);
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

void Player::takeDamage(int amount) {
  hp -= amount;
  if (hp < 0) hp = 0;
}

void Player::die() { hp = 0; }

void Player::draw(const Texture2D& texture) const {
  drawOffset(texture, pos, WHITE);
  DrawCircleLinesV(pos, radius, RED);
};

void playerShoot(GameState& state) {
  if (!IsKeyPressed(KEY_SPACE)) return;

  if (state.projectilePool.fire(state.player.pos))
    PlaySound(state.resources.laserSound);
}

void Projectile::spawn(Vector2 from, const ProjectileSpec& spec) {
  pos = from;
  this->dir = spec.dir;
  this->speed = spec.speed;
  this->radius = spec.radius;
  this->flipVertical = spec.flipVertical;
  active = true;
}

void Projectile::update(float dt) {
  if (!active) return;
  pos = Vector2Add(pos, Vector2Scale(dir, speed * dt));
  active = pos.y > 0 && pos.y < GetScreenHeight();
}

void Projectile::draw(const Texture2D& texture) const {
  if (!active) return;
  Rectangle sourceRect = {0.0f, 0.0f, (float)texture.width,
                          flipVertical ? -(float)texture.height
                                       : (float)texture.height};
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureRec(texture, sourceRect, drawPos, WHITE);
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
    state.enemyProjectilePool.fire(shooter->pos);
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
      state.player.die();
    }
  }
}

void detectEnemyBulletPlayerCollisions(GameState& state) {
  for (Projectile& bullet : state.enemyProjectilePool) {
    if (!bullet.active) continue;

    if (overlaps(state.player, bullet)) {
      bullet.deactivate();
      state.player.takeDamage();
    }
  }
}

std::unique_ptr<Scene> SceneGameplay::update(GameState& state, float dt) {
  // REFACTOR: the return value of swarm.update() is unclear
  // REFACTOR: should be state.player.die();
  if (state.swarm.update(dt)) state.player.die();
  state.player.update(dt);
  playerShoot(state);
  updateEnemyFire(state, dt);
  state.projectilePool.update(dt);
  state.enemyProjectilePool.update(dt);
  detectBulletEnemyCollisions(state);
  detectPlayerEnemyCollisions(state);
  detectEnemyBulletPlayerCollisions(state);

  if (state.swarm.activeCount() <= 0) state.victory = true;
  if (state.victory || !state.player.isAlive())
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
  drawHud(state.player.health(), state.score);

  EndDrawing();
}
