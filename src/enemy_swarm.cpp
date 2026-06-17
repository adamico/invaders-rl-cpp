#include "game.h"

#define MAX_ENEMIES_PER_ROW 11
#define COL_PADDING 80
#define ROW_PADDING 60

#define MAX_PROJECTILES 100

#define FOR_EACH_ENEMY(enemyPtr, enemyArray)                                   \
  for (Enemy *enemyPtr = enemyArray; enemyPtr < enemyArray + MAX_ENEMIES;      \
       enemyPtr++)

#define FOR_EACH_PROJECTILE(projectilePtr, projectileArray)                    \
  for (Projectile *projectilePtr = projectileArray;                            \
       projectilePtr < projectileArray + MAX_PROJECTILES; projectilePtr++)

void EnemySwarm::Init(Vector2 gridStartPos) {
  direction = (Vector2){1.0f, 0.0f};
  speed = ENEMY_SPEED;
  needToMoveDown = false;
  activeCount = 0;

  for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES; enemyIndex++) {
    int column = enemyIndex % MAX_ENEMIES_PER_ROW;
    int row = enemyIndex / MAX_ENEMIES_PER_ROW;
    float offsetX = gridStartPos.x;
    float offsetY = gridStartPos.y;
    Vector2 startPos = {offsetX + (column * COL_PADDING),
                        offsetY + (row * ROW_PADDING)};

    enemies[enemyIndex].Init(startPos);

    activeCount++;
  }
}

void EnemySwarm::Update(Vector2 dir, float speed, float dt) {
  FOR_EACH_ENEMY(enemy, enemies) {
    if (enemy->WillHitEdge(direction)) {
      direction.x *= -1.0f;
      needToMoveDown = true;
      break;
    }
  }

  if (needToMoveDown) {
    FOR_EACH_ENEMY(enemy, enemies)
    enemy->MoveDown(ROW_PADDING / 2.0f);

    needToMoveDown = false;
  }

  FOR_EACH_ENEMY(enemy, enemies)
  enemy->Update(direction, speed, dt);
}

void EnemySwarm::Draw(GameState *state) {
  FOR_EACH_ENEMY(enemy, enemies)
  enemy->Draw(state);
}

void EnemySwarm::HandleCollisions(GameState *state) {
  FOR_EACH_PROJECTILE(bullet, state->bullets) {
    if (!bullet->active)
      continue;

    FOR_EACH_ENEMY(enemy, enemies) {
      if (!enemy->active)
        continue;

      if (CheckCollisionCircles(bullet->pos, bullet->radius, enemy->pos,
                                enemy->radius)) {
        PlaySound(state->resources.explosionSound);
        bullet->active = false;
        enemy->active = false;
        activeCount--;
        state->score += enemy->scoreValue;
      }
    }
  }

  FOR_EACH_ENEMY(enemy, enemies) {
    if (!enemy->active)
      continue;

    if (CheckCollisionCircles(state->player.pos, state->player.radius,
                              enemy->pos, enemy->radius)) {
      enemy->active = false;
      state->currentScene = GAMEOVER;
    }
  }
}
