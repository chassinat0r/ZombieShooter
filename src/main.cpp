#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <engine/texturemgr.h>
#include <engine/sprite.h>
#include <engine/animation.h>
#include <engine/keyboard.h>

#include <stdio.h>

#include <global.h>
#include <constants.h>

#include <iostream>
#include <chrono>
#include <thread>

bool running = false;
bool playerMoving = false;
int playerDir = 0;
// 0 forward, 1 backward, 2 left, 3 right
// int count = 0;

GLFWwindow* window;

Sprite *player;
Sprite *zombie;

Camera camera = {0, 0, 0};

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    Global::width = w;
    Global::height = h;
    glViewport(0, 0, Global::width, Global::height);
}

void handleInput() {
    glfwPollEvents();

    if (Keyboard::isKeyPressed(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_W)) {
        if (playerDir != 1) {
            playerDir = 1;
        }
        player->setAnimation("back_walk");
        playerMoving = true;
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_S)) {
        if (playerDir != 0) {
            playerDir = 0;
        }
        player->setAnimation("front_walk");
        playerMoving = true;
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_A)) {
        if (playerDir != 2) {
            playerDir = 2;
        }
        player->setAnimation("left_walk");
        playerMoving = true;
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_D)) {
        if (playerDir != 3) {
            playerDir = 3;
        }
        player->setAnimation("right_walk");
        playerMoving = true;
    }

    if (Keyboard::isKeyReleased(window, GLFW_KEY_W)) {
        if (playerDir == 1) {
            player->setAnimation("back_still");
            playerMoving = false;
        }
    }

    if (Keyboard::isKeyReleased(window, GLFW_KEY_S)) {
        if (playerDir == 0) {
            player->setAnimation("front_still");
            playerMoving = false;
        }
    }

    if (Keyboard::isKeyReleased(window, GLFW_KEY_A)) {
        if (playerDir == 2) {
            player->setAnimation("left_still");
            playerMoving = false;
        }
    }

    if (Keyboard::isKeyReleased(window, GLFW_KEY_D)) {
        if (playerDir == 3) {
            player->setAnimation("right_still");
            playerMoving = false;
        }
    }
}

void update() {
    if (playerMoving) {
        if (playerDir == 0) {
            player->move(0, -20);
        } else if (playerDir == 1) {
            player->move(0, 20);
        } else if (playerDir == 2) {
            player->move(-20, 0);
        } else if (playerDir == 3) {
            player->move(20, 0);
        }
    }

    zombie->update();
    player->update();

    // if (player->isCollidingWith(*zombie)) {
    //     printf("Player colliding!\n");
    //     // printf("Collision %d!\n", count);
    //     // count++;
    // }
    camera.x = (int)player->getX();
    camera.y = (int)player->getY();

    running = !glfwWindowShouldClose(window);
}

void draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    zombie->draw(camera);
    player->draw(camera);
    glfwSwapBuffers(window);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "My Game", NULL, NULL);

    if (window == NULL) {
        printf("Error creating GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Error initialising GLAD\n");
        return -1;
    }    
    
    glViewport(0, 0, Global::width, Global::height);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    running = true;

    TextureManager::init();
    TextureManager::loadTex("assets/player.png", "player", 4, 4);
    TextureManager::loadTex("assets/test.png", "test", 2, 3);

    player = new Sprite(0, 0, 1.0f);
    Animation frontStill("front_still");
    frontStill.addFrame("player", 0, 0, 1, 1, 700);
    frontStill.addFrame("player", 0, 1, 1, 2, 700);

    Animation backStill("back_still");
    backStill.addFrame("player", 1, 0, 2, 1, 700);
    backStill.addFrame("player", 1, 1, 2, 2, 700);

    Animation leftStill("left_still");
    leftStill.addFrame("player", 3, 0, 4, 1, 700);
    leftStill.addFrame("player", 3, 1, 4, 2, 700);

    Animation rightStill("right_still");
    rightStill.addFrame("player", 2, 0, 3, 1, 700);
    rightStill.addFrame("player", 2, 1, 3, 2, 700);
    
    player->addAnimation("front_still", frontStill);
    player->addAnimation("back_still", backStill);
    player->addAnimation("left_still", leftStill);
    player->addAnimation("right_still", rightStill);

    Animation frontWalk("front_walk");
    frontWalk.addFrame("player", 0, 2, 1, 3, 200);
    frontWalk.addFrame("player", 0, 3, 1, 4, 200);

    Animation backWalk("back_walk");
    backWalk.addFrame("player", 1, 2, 2, 3, 200);
    backWalk.addFrame("player", 1, 3, 2, 4, 200);

    Animation leftWalk("left_walk");
    leftWalk.addFrame("player", 3, 2, 4, 3, 200);
    leftWalk.addFrame("player", 3, 1, 4, 2, 100);
    leftWalk.addFrame("player", 3, 3, 4, 4, 200);
    leftWalk.addFrame("player", 3, 1, 4, 2, 100);

    Animation rightWalk("right_walk");
    rightWalk.addFrame("player", 2, 2, 3, 3, 200);
    rightWalk.addFrame("player", 2, 1, 3, 2, 100);
    rightWalk.addFrame("player", 2, 3, 3, 4, 200);
    rightWalk.addFrame("player", 2, 1, 3, 2, 100);

    player->addAnimation("front_walk", frontWalk);
    player->addAnimation("back_walk", backWalk);
    player->addAnimation("left_walk", leftWalk);
    player->addAnimation("right_walk", rightWalk);

    // front still
    // this was for debugging collision, commented it but not deleting it as it may be needed in the future
    // player->addHitbox("front_still", 0, 0, 0, 16, 25);
    // player->addHitbox("front_still", 1, 0, 0, 16, 25);

    player->addHitbox("front_still", 0, 3, 0, 12, 9);
    player->addHitbox("front_still", 0, 1, 9, 14, 16);
    player->addHitbox("front_still", 0, 3, 16, 12, 24);

    player->addHitbox("front_still", 1, 3, 1, 12, 10);
    player->addHitbox("front_still", 1, 1, 10, 14, 17);
    player->addHitbox("front_still", 1, 3, 17, 12, 24);

    // back still
    player->addHitbox("back_still", 0, 3, 0, 12, 9);
    player->addHitbox("back_still", 0, 1, 9, 14, 16);
    player->addHitbox("back_still", 0, 3, 16, 12, 24);

    player->addHitbox("back_still", 1, 3, 1, 12, 10);
    player->addHitbox("back_still", 1, 1, 10, 14, 17);
    player->addHitbox("back_still", 1, 3, 17, 12, 24);

    // right still
    player->addHitbox("right_still", 0, 3, 0, 12, 9);
    player->addHitbox("right_still", 0, 6, 9, 10, 24);

    player->addHitbox("right_still", 1, 3, 1, 12, 10);
    player->addHitbox("right_still", 1, 6, 10, 10, 24);

    // left still first frame
    player->addHitbox("left_still", 0, 3, 0, 12, 9);
    player->addHitbox("left_still", 0, 5, 9, 9, 24);
    // second frame
    player->addHitbox("left_still", 1, 3, 1, 12, 10);
    player->addHitbox("left_still", 1, 5, 10, 9, 24);

    // front walk
    // frame 1
    player->addHitbox("front_walk", 0, 3, 1, 12, 10);
    player->addHitbox("front_walk", 0, 1, 10, 12, 16);
    player->addHitbox("front_walk", 0, 12, 10, 14, 14);
    player->addHitbox("front_walk", 0, 3, 16, 7, 22);
    player->addHitbox("front_walk", 0, 8, 16, 12, 24);

    // frame 2
    player->addHitbox("front_walk", 1, 3, 1, 12, 10);
    player->addHitbox("front_walk", 1, 1, 10, 3, 14);
    player->addHitbox("front_walk", 1, 3, 10, 14, 16);
    player->addHitbox("front_walk", 1, 3, 16, 7, 24);
    player->addHitbox("front_walk", 1, 8, 16, 12, 22);

    // back walk
    // frame 1
    player->addHitbox("back_walk", 0, 3, 1, 12, 10);
    player->addHitbox("back_walk", 0, 1, 10, 12, 16);
    player->addHitbox("back_walk", 0, 12, 10, 14, 14);
    player->addHitbox("back_walk", 0, 3, 16, 7, 22);
    player->addHitbox("back_walk", 0, 8, 16, 12, 24);

    // frame 2
    player->addHitbox("back_walk", 1, 3, 1, 12, 10);
    player->addHitbox("back_walk", 1, 1, 10, 3, 14);
    player->addHitbox("back_walk", 1, 3, 10, 14, 16);
    player->addHitbox("back_walk", 1, 3, 16, 7, 24);
    player->addHitbox("back_walk", 1, 8, 16, 12, 22);

    // left walk
    // frame 1
    player->addHitbox("left_walk", 0, 3, 1, 12, 10);
    player->addHitbox("left_walk", 0, 4, 10, 10, 17);
    player->addHitbox("left_walk", 0, 5, 17, 9, 21);
    player->addHitbox("left_walk", 0, 5, 21, 11, 24);

    // frame 2
    player->addHitbox("left_walk", 1, 3, 1, 12, 10);
    player->addHitbox("left_walk", 1, 5, 10, 9, 24);

    // frame 3
    player->addHitbox("left_walk", 2, 3, 1, 12, 10);
    player->addHitbox("left_walk", 2, 4, 10, 10, 16);
    player->addHitbox("left_walk", 2, 5, 16, 9, 22);
    player->addHitbox("left_walk", 2, 3, 22, 9, 24);

    // frame 4
    player->addHitbox("left_walk", 3, 3, 1, 12, 10);
    player->addHitbox("left_walk", 3, 5, 10, 9, 24);

    // right walk
    // frame 1
    player->addHitbox("right_walk", 0, 3, 1, 12, 10);
    player->addHitbox("right_walk", 0, 5, 10, 11, 17);
    player->addHitbox("right_walk", 0, 6, 17, 9, 21);
    player->addHitbox("right_walk", 0, 4, 21, 10, 24);

    // frame 2
    player->addHitbox("right_walk", 1, 3, 1, 12, 10);
    player->addHitbox("right_walk", 1, 6, 10, 10, 24);

    // frame 3
    player->addHitbox("right_walk", 2, 3, 1, 12, 10);
    player->addHitbox("right_walk", 2, 6, 10, 11, 16);
    player->addHitbox("right_walk", 2, 6, 16, 10, 22);
    player->addHitbox("right_walk", 2, 6, 22, 12, 24);

    // frame 4
    player->addHitbox("right_walk", 3, 3, 1, 12, 10);
    player->addHitbox("right_walk", 3, 6, 10, 10, 24);

    player->setAnimation("front_still");

    zombie = new Sprite(0, 0, 0.5f);
    Animation zombieStill("zombie_still");
    zombieStill.addFrame("test", 0, 0, 1, 1, 200);
    zombieStill.addFrame("test", 0, 1, 1, 2, 200);
    zombieStill.addFrame("test", 0, 2, 1, 3, 200);
    zombieStill.addFrame("test", 1, 0, 2, 1, 200);
    zombieStill.addFrame("test", 1, 1, 2, 2, 200);
    zombieStill.addFrame("test", 1, 2, 2, 3, 200);
    zombie->addAnimation("zombie_still", zombieStill);
    zombie->addHitbox("zombie_still", 0, 0, 0, 40, 40);
    zombie->addHitbox("zombie_still", 1, 0, 0, 40, 40);
    zombie->addHitbox("zombie_still", 2, 0, 0, 40, 40);
    zombie->addHitbox("zombie_still", 3, 0, 0, 40, 40);
    zombie->addHitbox("zombie_still", 4, 0, 0, 40, 40);
    zombie->addHitbox("zombie_still", 5, 0, 0, 40, 40);
    zombie->setAnimation("zombie_still");


    while (running) {
        double time_to_wait = FRAME_TARGET_TIME - (1000*glfwGetTime() - Global::last_frame_time);

        if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
            std::this_thread::sleep_for(std::chrono::milliseconds((int)time_to_wait));
        }

        handleInput();
        update();
        draw();

        Global::last_frame_time = 1000*glfwGetTime();
    }

    glfwTerminate();
    
    return 0;
}
