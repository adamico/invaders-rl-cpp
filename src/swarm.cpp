#include "swarm.h"

constexpr int MAX_ENEMIES_PER_ROW = 11;
constexpr int COL_PADDING = 80;
constexpr int ROW_PADDING = 60;
constexpr float ENEMY_SPEED = 50.0f;
constexpr float ENEMY_RADIUS = 15.0f;
constexpr int ENEMY_SCORE_VALUE = 100;
constexpr float ENEMY_VERTICAL_MOVEMENT = ROW_PADDING / 2.0f;

void Swarm::reset() {
  direction = {1.0f, 0.0f};
  speed = ENEMY_SPEED;
  Vector2 startGridPos = {
      ((GetScreenWidth() - (MAX_ENEMIES_PER_ROW * COL_PADDING)) / 2.0f) +
          (COL_PADDING / 2.0f),
      100};

  for (int enemyIndex = 0; enemyIndex < MAX_ENEMIES; enemyIndex++) {
    int col = enemyIndex % MAX_ENEMIES_PER_ROW;
    int row = enemyIndex / MAX_ENEMIES_PER_ROW;
    float offsetX = startGridPos.x;
    float offsetY = startGridPos.y;
    pool.items[enemyIndex] = {
        .pos = {offsetX + (col * COL_PADDING), offsetY + (row * ROW_PADDING)},
        .radius = ENEMY_RADIUS,
        .scoreValue = ENEMY_SCORE_VALUE,
        .active = true,
    };
  }
}

bool Swarm::update(float deltaTime) {
  bool needToMoveDown = false;
  bool breached = false;
  int rightEdge = GetScreenWidth() - ENEMY_RADIUS;
  int leftEdge = ENEMY_RADIUS;
  for (Enemy& enemy : pool.items) {
    if (!enemy.active) continue;

    bool willHitRightEdge =
        (enemy.pos.x + enemy.radius) >= rightEdge && direction.x > 0;
    bool willHitLeftEdge =
        (enemy.pos.x - enemy.radius) <= leftEdge && direction.x < 0;
    if (willHitRightEdge || willHitLeftEdge) {
      direction.x *= -1.0f;
      needToMoveDown = true;
      break;
    }
  }

  if (needToMoveDown) {
    for (Enemy& enemy : pool.items) {
      enemy.moveVertically(ENEMY_VERTICAL_MOVEMENT);
      if (enemy.pos.y > GetScreenHeight()) {
        breached = true;
        break;
      }
    }
  }

  for (Enemy& enemy : pool.items) {
    if (!enemy.active) continue;

    enemy.moveHorizontally(direction, speed, deltaTime);
  }

  return breached;
}

void Swarm::draw(const Texture2D& texture) const {
  for (const Enemy& enemy : pool.items) {
    enemy.draw(texture);
  }
}

void Swarm::deactivate(Enemy& enemy) { enemy.active = false; }
