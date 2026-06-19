#pragma once

#include "pool.h"
#include <raylib.h>

constexpr int BUNKER_NUMBER = 3;
constexpr int MAX_BLOCKS = 24;
constexpr int MAX_BLOCK_PER_ROW = 6;
constexpr int BLOCK_SIZE = 16.0f;

struct Block {
  Vector2 pos;
  float radius;
  bool active;

  void deactivate() { active = false; }
};

class Bunker {
public:
  void reset(Vector2 position);
  void draw() const;

  auto begin() { return pool.begin(); }
  auto end() { return pool.end(); }
  auto begin() const { return pool.begin(); }
  auto end() const { return pool.end(); }

private:
  Pool<Block, MAX_BLOCKS> pool{};
};
