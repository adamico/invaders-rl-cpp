#include "projectile.h"

#include "canvas.h"
#include <raymath.h>

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
