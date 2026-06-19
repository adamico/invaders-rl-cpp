#include "scene_gameplay.h"

#include "collision.h"
#include "hud.h"
#include "projectile_pool.h"
#include "raylib.h"
#include "resources.h"
#include "scene_gameover.h"

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

namespace {
void playerShoot(GameState& state) {
  if (!IsKeyPressed(KEY_SPACE)) return;

  if (state.projectilePool.fire(state.player.pos))
    PlaySound(state.resources.laserSound);
}

void updateEnemyFire(GameState& state, float dt) {
  state.enemyFireCooldown -= dt;
  if (state.enemyFireCooldown > 0.0f) return;

  if (auto pos = state.swarm.randomShooterPosition()) {
    state.enemyProjectilePool.fire(*pos);
  }

  state.enemyFireCooldown = ENEMY_FIRE_COOLDOWN;
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

void detectProjectileBlockCollisions(ProjectilePool& pool, GameState& state) {
  for (Projectile& bullet : pool) {
    if (!bullet.active) continue;

    for (Bunker& bunker : state.bunkers) {
      for (Block& block : bunker) {
        if (!block.active) continue;

        if (overlaps(bullet, block)) {
          bullet.deactivate();
          block.deactivate();
          break;
        }
      }
    }
  }
}

void detectEnemyBlockCollisions(GameState& state) {
  for (Enemy& enemy : state.swarm) {
    if (!enemy.active) continue;

    for (Bunker& bunker : state.bunkers) {
      for (Block& block : bunker) {
        if (!block.active) continue;

        if (overlaps(enemy, block)) {
          block.deactivate();
        }
      }
    }
  }
}

} // namespace

void SceneGameplay::enter(GameState& state) {
  state.score = 0;
  state.victory = false;

  state.player.reset();
  state.swarm.reset();
  float bunkerY = GetScreenHeight() - 220.0f;
  float bunkerSpacing = 300.0f;
  float bunkerWidth = MAX_BLOCK_PER_ROW * BLOCK_SIZE;
  float bunkersWidth =
      (bunkerWidth * BUNKER_NUMBER) + (bunkerSpacing * (BUNKER_NUMBER - 1));
  float offsetX = (GetScreenWidth() - bunkersWidth) / 2.0f;
  Vector2 startBunkerPos = {offsetX, bunkerY};
  for (int bunkerIndex = 0; bunkerIndex < BUNKER_NUMBER; ++bunkerIndex) {
    state.bunkers[bunkerIndex].reset(
        {startBunkerPos.x + (bunkerWidth + bunkerSpacing) * bunkerIndex,
         startBunkerPos.y});
  }
  state.projectilePool.reset(PLAYER_PROJECTILE_SPEC);
  state.enemyProjectilePool.reset(ENEMY_PROJECTILE_SPEC);
  state.enemyFireCooldown = ENEMY_FIRE_COOLDOWN;
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
  detectProjectileBlockCollisions(state.projectilePool, state);
  detectProjectileBlockCollisions(state.enemyProjectilePool, state);
  detectEnemyBlockCollisions(state);

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
  for (const auto& bunker : state.bunkers) {
    bunker.draw();
  }
  drawHud(state.player.livesRemaining(), state.score);

  EndDrawing();
}
