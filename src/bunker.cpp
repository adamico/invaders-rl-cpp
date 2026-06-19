#include "bunker.h"
#include "raylib.h"

void Bunker::reset(Vector2 origin) {
  for (int blockIndex = 0; blockIndex < MAX_BLOCKS; ++blockIndex) {
    int col = blockIndex % MAX_BLOCK_PER_ROW;
    int row = blockIndex / MAX_BLOCK_PER_ROW;

    Block& block = pool[blockIndex];
    block = {
        .pos = {origin.x + (col * BLOCK_SIZE), origin.y + (row * BLOCK_SIZE)},
        .radius = BLOCK_SIZE / 2.0f,
        .active = true};
  }
};

void Bunker::draw() const {
  pool.eachActive([&](const Block& block) {
    DrawRectangle(block.pos.x, block.pos.y, BLOCK_SIZE, BLOCK_SIZE, YELLOW);
  });
}
