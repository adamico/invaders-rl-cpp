#include "bunker.h"
#include "raylib.h"
#include <array>

void Bunker::reset(Vector2 origin) {
  std::array<int, MAX_BLOCKS> blockGrid = {
      0, 1, 1, 1, 1, 0,
      1, 1, 1, 1, 1, 1,
      1, 1, 1, 1, 1, 1,
      1, 1, 0, 0, 1, 1,
  };
  for (int gridPosition = 0; gridPosition < MAX_BLOCKS; ++gridPosition) {
    int col = gridPosition % MAX_BLOCK_PER_ROW;
    int row = gridPosition / MAX_BLOCK_PER_ROW;

    Block& block = pool[gridPosition];

    block = {
        .pos = {origin.x + (col * BLOCK_SIZE), origin.y + (row * BLOCK_SIZE)},
        .radius = BLOCK_SIZE / 2.0f,
        .active = blockGrid[gridPosition] == 1};
  }
};

void Bunker::draw() const {
  pool.eachActive([&](const Block& block) {
    DrawRectangle(block.pos.x, block.pos.y, BLOCK_SIZE, BLOCK_SIZE, YELLOW);
  });
}
