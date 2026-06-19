#include <stdarg.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "canvas.h"
#include "collision.h"
#include "game.h"
#include "hud.h"
#include "projectile_pool.h"
#include "scene_gameover.h"
#include "scene_gameplay.h"

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

void initGameplay(GameState* state) {
  state->score = 0;
  state->victory = false;

  state->player.reset();
  state->swarm.reset();
  state->projectilePool.reset(PLAYER_PROJECTILE_SPEC);
  state->enemyProjectilePool.reset(ENEMY_PROJECTILE_SPEC);
  state->enemyFireCooldown = ENEMY_FIRE_COOLDOWN;
}

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

  if (auto pos = state.swarm.randomShooterPosition()) {
    state.enemyProjectilePool.fire(*pos);
  }

  state.enemyFireCooldown = ENEMY_FIRE_COOLDOWN;
}

void Enemy::moveHorizontally(Vector2 dir, float speed, float deltaTime) {
  pos = Vector2Add(pos, Vector2Scale(dir, speed * deltaTime));
}

void Enemy::moveVertically(float amount) { pos.y += amount; }

void Enemy::draw(const Texture2D& texture) const {
  if (!active) return;
  Vector2 drawPos = Vector2Add(pos, CANVAS_OFFSET);
  DrawTextureV(texture, drawPos, WHITE);
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
      state.player.takeDamage();
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
  state.player.update(dt);
  playerShoot(state);
  state.projectilePool.update(dt);

  state.swarm.update(dt);
  updateEnemyFire(state, dt);
  state.enemyProjectilePool.update(dt);

  detectBulletEnemyCollisions(state);
  detectPlayerEnemyCollisions(state);
  detectEnemyBulletPlayerCollisions(state);

  if (state.swarm.hasBreached()) state.player.die();
  if (state.swarm.activeCount() <= 0) state.victory = true;
  if (state.victory || state.player.isDead())
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
  drawHud(state.player.livesRemaining(), state.score);

  EndDrawing();
}
