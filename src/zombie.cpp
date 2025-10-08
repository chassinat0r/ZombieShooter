#include <zombie.h>
#include <global.h>

#include <GLFW/glfw3.h>

Zombie::Zombie(float x, float y, int healthMax, int healthCurr, float scale, bool solid) : Sprite(x, y, scale, solid) {
    this->health = healthCurr;
    this->healthMax = healthMax;

    Animation idleLeft("idle_left");
    idleLeft.addFrame("zombie", 1, 0, 2, 1, 700);
    idleLeft.addFrame("zombie", 1, 1, 2, 2, 700);
    addAnimation("idle_left", idleLeft);

    Animation idleRight("idle_right");
    idleRight.addFrame("zombie", 0, 0, 1, 1, 700);
    idleRight.addFrame("zombie", 0, 1, 1, 2, 700);
    addAnimation("idle_right", idleRight);

    // idle left
    // frame 0
    addHitbox("idle_left", 0, 4, 1, 12, 7);
    addHitbox("idle_left", 0, 4, 7, 13, 9);
    addHitbox("idle_left", 0, 7, 9, 13, 12);
    addHitbox("idle_left", 0, 7, 12, 13, 15);

    // frame 1
    addHitbox("idle_left", 1, 4, 2, 12, 8);
    addHitbox("idle_left", 1, 5, 9, 6, 9);
    addHitbox("idle_left", 1, 7, 8, 13, 12);
    addHitbox("idle_left", 1, 7, 12, 13, 15);

    // idle right
    // frame 0
    addHitbox("idle_right", 0, 3, 1, 11, 7);
    addHitbox("idle_right", 0, 2, 7, 11, 9);
    addHitbox("idle_right", 0, 2, 9, 8, 11);
    addHitbox("idle_right", 0, 2, 11, 8, 15);

    // frame 1
    addHitbox("idle_right", 1, 3, 2, 11, 8);
    addHitbox("idle_right", 1, 9, 9, 10, 9);
    addHitbox("idle_right", 1, 2, 8, 8, 11);
    addHitbox("idle_right", 1, 2, 11, 8, 15);

    // set starting animation
    setAnimation("idle_left");
}

Zombie::Zombie() {

}

void Zombie::update() {
    Sprite::update();

    if (timeOut) {
        cdProgress += (glfwGetTime()*1000 - Global::last_frame_time);
        if (cdProgress > cooldown) {
            timeOut = false;
            cdProgress = 0;
        }
    }
}

void Zombie::registerAttack() {
    timeOut = true;
    cdProgress = 0;
}

bool Zombie::isInCooldown() { return timeOut; }
