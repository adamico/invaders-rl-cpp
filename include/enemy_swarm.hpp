#pragma once

#include "raylib.h"

#include "enemy.hpp"

#define MAX_ENEMIES 55

class EnemySwarm {
public:
    EnemySwarm() {};
    void Init(Vector2 startPos);
    void Update(Vector2 dir, float speed, float dt);
    void Draw(struct GameState* state);
    void HandleCollisions(struct GameState* state);

    Enemy enemies[MAX_ENEMIES];
    Vector2 direction;
    int speed;
    bool needToMoveDown;
    int activeCount;
};
