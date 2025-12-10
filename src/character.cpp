#include <character.h>

void Character::setHealth(int health) {
    this->health = health;
}

void Character::setMaxHealth(int healthMax) {
    this->healthMax = healthMax;
}

void Character::addHealth(int change) {
    health += change;
    if (health > healthMax) {
        health = healthMax;
    }
}

void Character::removeHealth(int change) {
    health -= change;
    if (health < 0) {
        health = 0;
    }
}
