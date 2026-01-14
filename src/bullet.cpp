#include <bullet.h>
#include <zombie.h>

#include <algorithm>
#include <cmath>

Bullet::Bullet(float x, float y, float angle, float scale) : Sprite(x, y, "bullet", scale, true) {
    Animation bullet("bullet");
    bullet.addFrame("bullet", 0, 0, 1, 1, 100);
    addAnimation("bullet", bullet);

    addHitbox("bullet", 0, 0, 0, 5, 1);

    setAnimation("bullet");

    this->angle = angle;

    bullets.push_back(this);
}

Bullet::Bullet() {

}

void Bullet::update() {
    velocityX = speed * cos(angle);
    velocityY = -speed * sin(angle);
    Sprite::update();

    bool hit = false;
    for (Sprite *sc : spriteCollisions) {
        BulletHitListener *cast = dynamic_cast<BulletHitListener*>(sc);
        if (cast != nullptr) {
            new BulletHitEvent(cast);
            hit = true;
        }
    }

    if (hit) {
        bullets.erase(std::remove(bullets.begin(), bullets.end(), this), bullets.end());
        sprites.erase(std::remove(sprites.begin(), sprites.end(), this), sprites.end());

    }
}
