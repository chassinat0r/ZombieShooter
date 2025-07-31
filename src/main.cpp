#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <engine/texturemgr.h>
#include <engine/sprite.h>
#include <engine/animation.h>

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

GLFWwindow* window;

Sprite *player;

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    Global::width = w;
    Global::height = h;
    glViewport(0, 0, Global::width, Global::height);
}

void handleInput() {
    glfwPollEvents();

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        if (playerDir != 1) {
            player->setAnimation("back_still");
            playerDir = 1;
        }
        playerMoving = true;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        if (playerDir != 0) {
            player->setAnimation("front_still");
            playerDir = 0;
        }
        playerMoving = true;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        if (playerDir != 2) {
            player->setAnimation("left_still");
            playerDir = 2;
        }
        playerMoving = true;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        if (playerDir != 3) {
            player->setAnimation("right_still");
            playerDir = 3;
        }
        playerMoving = true;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {
        if (playerDir == 1) {
            playerMoving = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE) {
        if (playerDir == 0) {
            playerMoving = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE) {
        if (playerDir == 2) {
            playerMoving = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE) {
        if (playerDir == 3) {
            playerMoving = false;
        }
    }
}

void update() {
    if (playerMoving) {
        if (playerDir == 0) {
            player->move(0, -30);
        } else if (playerDir == 1) {
            player->move(0, 30);
        } else if (playerDir == 2) {
            player->move(-30, 0);
        } else if (playerDir == 3) {
            player->move(30, 0);
        }
    }

    running = !glfwWindowShouldClose(window);
}

void draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Camera camera;
    camera.x = 0;
    camera.y = 0;
    camera.angle = 0;

    player->draw(camera, 3.0f);
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

    player = new Sprite(0, 0);
    Animation frontStill("front_still");
    frontStill.addFrame("player", 0, 0, 1, 1, 500);
    frontStill.addFrame("player", 0, 1, 1, 2, 500);

    Animation backStill("back_still");
    backStill.addFrame("player", 1, 0, 2, 1, 500);
    backStill.addFrame("player", 1, 1, 2, 2, 500);

    Animation leftStill("left_still");
    leftStill.addFrame("player", 3, 0, 4, 1, 500);
    leftStill.addFrame("player", 3, 1, 4, 2, 500);

    Animation rightStill("right_still");
    rightStill.addFrame("player", 2, 0, 3, 1, 500);
    rightStill.addFrame("player", 2, 1, 3, 2, 500);
    
    player->addAnimation("front_still", frontStill);
    player->addAnimation("back_still", backStill);
    player->addAnimation("left_still", leftStill);
    player->addAnimation("right_still", rightStill);
    player->setAnimation("front_still");

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
