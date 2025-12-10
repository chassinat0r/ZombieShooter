#include <engine/texturemgr.h>

#include <player.h>
#include <constants.h>
#include <global.h>
#include <util.h>

Player::Player(float x, float y, float scale, bool solid) : Character(x, y, "player", scale, solid) {
    Animation playerIdleLeft("idle_left");
    playerIdleLeft.addFrame("player", 1, 0, 2, 1, 700);
    playerIdleLeft.addFrame("player", 1, 1, 2, 2, 700);
    addAnimation("idle_left", playerIdleLeft);

    Animation playerIdleRight("idle_right");
    playerIdleRight.addFrame("player", 0, 0, 1, 1, 700);
    playerIdleRight.addFrame("player", 0, 1, 1, 2, 700);
    addAnimation("idle_right", playerIdleRight);

    Animation playerFallLeft("fall_left");
    playerFallLeft.addFrame("player", 1, 1, 2, 2, 700);
    addAnimation("fall_left", playerFallLeft);

    Animation playerFallRight("fall_right");
    playerFallRight.addFrame("player", 0, 1, 1, 2, 700);
    addAnimation("fall_right", playerFallRight);

    Animation playerWalkLeft("walk_left");
    playerWalkLeft.addFrame("player", 1, 0, 2, 1, 300);
    playerWalkLeft.addFrame("player", 1, 2, 2, 3, 300);
    addAnimation("walk_left", playerWalkLeft);

    Animation playerWalkRight("walk_right");
    playerWalkRight.addFrame("player", 0, 0, 1, 1, 300);
    playerWalkRight.addFrame("player", 0, 2, 1, 3, 300);
    addAnimation("walk_right", playerWalkRight);
    
    Animation playerJumpLeft("jump_left");
    playerJumpLeft.addFrame("player", 1, 3, 2, 4, 700);
    addAnimation("jump_left", playerJumpLeft);

    Animation playerJumpRight("jump_right");
    playerJumpRight.addFrame("player", 0, 3, 1, 4, 700);
    addAnimation("jump_right", playerJumpRight);

    // idle left
    // frame 0
    addHitbox("idle_left", 0, 3, 0, 11, 6);
    addHitbox("idle_left", 0, 4, 6, 10, 11);
    addHitbox("idle_left", 0, 4, 11, 10, 15);

    // frame 1
    addHitbox("idle_left", 1, 3, 1, 11, 7);
    addHitbox("idle_left", 1, 4, 7, 10, 11);
    addHitbox("idle_left", 1, 4, 11, 10, 15);

    // idle right
    // frame 0
    addHitbox("idle_right", 0, 4, 0, 12, 6);
    addHitbox("idle_right", 0, 5, 6, 11, 11);
    addHitbox("idle_right", 0, 5, 11, 11, 15);

    // frame 1
    addHitbox("idle_right", 1, 4, 1, 12, 7);
    addHitbox("idle_right", 1, 5, 7, 11, 11);
    addHitbox("idle_right", 1, 5, 11, 11, 15);

    // walk left
    // frame 0
    addHitbox("walk_left", 0, 3, 0, 11, 6);
    addHitbox("walk_left", 0, 4, 6, 10, 11);
    addHitbox("walk_left", 0, 4, 11, 10, 15);

    // frame 1
    addHitbox("walk_left", 1, 3, 0, 11, 6);
    addHitbox("walk_left", 1, 4, 6, 10, 11);
    addHitbox("walk_left", 1, 5, 11, 9, 15);

    // walk right
    // frame 0
    addHitbox("walk_right", 0, 4, 0, 12, 6);
    addHitbox("walk_right", 0, 5, 6, 11, 11);
    addHitbox("walk_right", 0, 5, 11, 11, 15);

    // frame 1
    addHitbox("walk_right", 1, 4, 0, 12, 6);
    addHitbox("walk_right", 1, 5, 6, 11, 11);
    addHitbox("walk_right", 1, 6, 11, 10, 15);

    // jump left
    // frame 0
    addHitbox("jump_left", 0, 3, 0, 11, 6);
    addHitbox("jump_left", 0, 4, 6, 10, 10);
    addHitbox("jump_left", 0, 4, 10, 10, 12);

    // jump right
    // frame 0
    addHitbox("jump_right", 0, 4, 0, 12, 6);
    addHitbox("jump_right", 0, 5, 6, 11, 10);
    addHitbox("jump_right", 0, 5, 10, 11, 12);

    // fall left
    // frame 0
    addHitbox("fall_left", 0, 3, 1, 11, 7);
    addHitbox("fall_left", 0, 4, 7, 10, 11);
    addHitbox("fall_left", 0, 4, 11, 10, 15);

    // fall right
    // frame 0
    addHitbox("fall_right", 0, 4, 1, 12, 7);
    addHitbox("fall_right", 0, 5, 7, 11, 11);
    addHitbox("fall_right", 0, 5, 11, 11, 15);

    // Set idle_right as starting animation
    setAnimation("idle_right");
}

Player::Player() {

}

void Player::update() {
    if (moving) {
        if (direction == 1) {
            move(-speed, 0);
        } else {
            move(speed, 0);
        }
    }

    Sprite::update();

    if (velocityY < 0 && jumping) {
        jumping = false;
    }

    if (!jumping && !isGrounded() && !falling) {
        if (direction == 1) {
            setAnimation("fall_left");
        } else {
            setAnimation("fall_right");
        }
        falling = true;
    }

    if (isGrounded() && falling) {
        falling = false;
        if (direction == 1) {
            if (moving) {
                setAnimation("walk_left");
            } else {
                setAnimation("idle_left");
            }
        } else {
            if (moving) {
                setAnimation("walk_right");
            } else {
                setAnimation("idle_right");
            }
        }
    }

    if (x < 0) {
        x = 0;
    }

    if (y < -280) {
        y = -280;
        removeHealth();
    }

    if (health == 0) {
        die();
    }
}

void Player::drawHealthBar() {
    std::pair<float,float> wh = getRenderWidthAndHeight();
    float w = wh.first;
    float h = wh.second;

    float hbStart = -0.5f*w + TextureManager::getTexWidth("health-icon", 0, 1);
    TextureManager::setTex("health-bar", 0, 0, 1, 1, "left", "top");
    TextureManager::drawTex(hbStart, 0.5f*h - 4, 1.5f, 0.0f);
    
    int availableSpaces = TextureManager::getTexWidth("health-bar", 0, 1) - 2;
    int filledSpaces = ((float)health / (float)healthMax) * (float)availableSpaces;
    
    TextureManager::setTex("health-states", 0, 0, 1, 1, "left", "top");

    for (int i = 0; i < filledSpaces; i++) {
        TextureManager::drawTex(hbStart + ((i+1)*1.5f), 0.5f*h-4-1.5f, 1.5f, 0.0f);
    }

    TextureManager::setTex("health-states", 0, 1, 1, 2, "left", "top");

    for (int i = filledSpaces; i < availableSpaces; i++) {
        TextureManager::drawTex(hbStart + ((i+1)*1.5f), 0.5f*h-4-1.5f, 1.5f, 0.0f);
    }

    TextureManager::setTex("health-icon", 0, 0, 1, 1, "left", "top");
    TextureManager::drawTex(-0.5f*w + 1, 0.5f*h - 1, 1.5f, 0.0f);
}

void Player::startMoving() {
    moving = true;
    if (direction == 1) {
        setAnimation("walk_left");
    }  else {
        setAnimation("walk_right");
    }
}

void Player::stopMoving() {
    moving = false;
    if (direction == 1) {
        setAnimation("idle_left");
    } else {
        setAnimation("idle_right");
    }
}

void Player::jump() {
    Sprite::jump(jumpSpeed);
    if (direction == 1) {
        setAnimation("jump_left");
    } else {
        setAnimation("jump_right");
    }
    jumping = true;
}
        
void Player::setDirection(int dir) {
    if (dir > 2) { dir = 2; }
    this->direction = dir;
}

int Player::getDirection() { return direction; }

void Player::die() {
    x = 0;
    y = 55;
    velocityX = 0;
    velocityY = 0;
    health = healthMax;
}
