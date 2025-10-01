#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <engine/texturemgr.h>
#include <engine/sprite.h>
#include <engine/animation.h>
#include <engine/keyboard.h>
#include <engine/level.h>

#include <stdio.h>

#include <global.h>
#include <constants.h>

#include <iostream>
#include <chrono>
#include <thread> 

#include <zombie.h>

bool running = false;
bool jumping = false;
bool falling = false;
bool playerMoving = false;
int playerDir = 0;
// 0 none, 1 left, 2 right

GLFWwindow* window;

Sprite *player;

Camera camera = {0, 0, 0};

void quickSort(std::vector<Sprite*> *vec, int start, int end) {
    if (end <= start) { return; }

	int i = -1;
	int j = 0;

	int pivot = vec->at(end)->getY();

	while (j < end) {
		if (vec->at(j)->getY() > pivot) {
			i++;
			Sprite *temp = vec->at(j);
			vec->at(j) = vec->at(i);
			vec->at(i) = temp;
		}

		j++;
	}

	i++;

	Sprite *temp = vec->at(i);
	vec->at(i) = vec->at(end);
	vec->at(end) = temp;

	quickSort(vec, start, i-1);
	quickSort(vec, i+1, end);
}

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

    if (Keyboard::isKeyPressed(window, GLFW_KEY_A)) {
        playerDir = 1;
        player->setAnimation("walk_left");
        playerMoving = true;
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_D)) {
        playerDir = 2;
        player->setAnimation("walk_right");
        playerMoving = true;
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_SPACE)) {
        if (player->isGrounded()) {
            player->jump();
            if (playerDir == 1) {
                player->setAnimation("jump_left");
            } else {
                player->setAnimation("jump_right");
            }
            jumping = true;
        }
    }
    if (Keyboard::isKeyReleased(window, GLFW_KEY_SPACE)) {

    }
    if (Keyboard::isKeyReleased(window, GLFW_KEY_A)) {
        if (playerDir == 1) {
            playerMoving = false;
            player->setAnimation("idle_left");
        }
    }

    if (Keyboard::isKeyReleased(window, GLFW_KEY_D)) {
        if (playerDir == 2) {
            playerMoving = false;
            player->setAnimation("idle_right");
        }
    }
}

void update() {
    if (playerMoving) {
        float speed = 30.0f;
        if (playerDir == 1) {
            player->move(-speed, 0);
        } else if (playerDir == 2) {
            player->move(speed, 0);
        }
    }

    player->update();

    if (player->getVelocityY() < 0 && jumping) {
        jumping = false;
    }

    if (!jumping && !player->isGrounded() && !falling) {
        if (playerDir == 1) {
            player->setAnimation("fall_left");
        } else {
            player->setAnimation("fall_right");
        }
        falling = true;
    }

    if (player->isGrounded() && falling) {
        falling = false;
        if (playerDir == 1) {
            player->setAnimation("idle_left");
        } else {
            player->setAnimation("idle_right");
        }
    }

    if (player->getX() < 0) {
        player->setPos(0, player->getY());
    }

    if (player->getY() < -280) {
        player->setPos(0, 45);
    }

    camera.x = (int)player->getX();
    camera.y = (int)player->getY();

    running = !glfwWindowShouldClose(window);
}

void draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Global::level->render(camera);

    std::vector<Sprite*> sorted = Sprite::sprites;

    quickSort(&sorted, 0, sorted.size()-1);
    
    for (Sprite *s : sorted) {
        s->draw(camera);
    }

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
    TextureManager::loadTex("assets/hero.png", "player", 2, 4);
    TextureManager::loadTex("assets/brick.png", "brick", 1, 1);
    TextureManager::loadTex("assets/floor.png", "floor", 1, 1);
    TextureManager::loadTex("assets/debug.png", "debug", 1, 1);

    TextureManager::loadTex("assets/zombie.png", "zombie", 4, 4);

    player = new Sprite(0, 45, 1.0f, true);

    Animation playerIdleLeft("idle_left");
    playerIdleLeft.addFrame("player", 1, 0, 2, 1, 700);
    playerIdleLeft.addFrame("player", 1, 1, 2, 2, 700);
    player->addAnimation("idle_left", playerIdleLeft);

    Animation playerIdleRight("idle_right");
    playerIdleRight.addFrame("player", 0, 0, 1, 1, 700);
    playerIdleRight.addFrame("player", 0, 1, 1, 2, 700);
    player->addAnimation("idle_right", playerIdleRight);

    Animation playerFallLeft("fall_left");
    playerFallLeft.addFrame("player", 1, 1, 2, 2, 700);
    player->addAnimation("fall_left", playerFallLeft);

    Animation playerFallRight("fall_right");
    playerFallRight.addFrame("player", 0, 1, 1, 2, 700);
    player->addAnimation("fall_right", playerFallRight);

    Animation playerWalkLeft("walk_left");
    playerWalkLeft.addFrame("player", 1, 0, 2, 1, 300);
    playerWalkLeft.addFrame("player", 1, 2, 2, 3, 300);
    player->addAnimation("walk_left", playerWalkLeft);

    Animation playerWalkRight("walk_right");
    playerWalkRight.addFrame("player", 0, 0, 1, 1, 300);
    playerWalkRight.addFrame("player", 0, 2, 1, 3, 300);
    player->addAnimation("walk_right", playerWalkRight);
    
    Animation playerJumpLeft("jump_left");
    playerJumpLeft.addFrame("player", 1, 3, 2, 4, 700);
    player->addAnimation("jump_left", playerJumpLeft);

    Animation playerJumpRight("jump_right");
    playerJumpRight.addFrame("player", 0, 3, 1, 4, 700);
    player->addAnimation("jump_right", playerJumpRight);

    // idle left
    // frame 0
    player->addHitbox("idle_left", 0, 3, 0, 11, 6);
    player->addHitbox("idle_left", 0, 4, 6, 10, 11);
    player->addHitbox("idle_left", 0, 4, 11, 10, 15);

    // frame 1
    player->addHitbox("idle_left", 1, 3, 1, 11, 7);
    player->addHitbox("idle_left", 1, 4, 7, 10, 11);
    player->addHitbox("idle_left", 1, 4, 11, 10, 15);

    // idle right
    // frame 0
    player->addHitbox("idle_right", 0, 4, 0, 12, 6);
    player->addHitbox("idle_right", 0, 5, 6, 11, 11);
    player->addHitbox("idle_right", 0, 5, 11, 11, 15);

    // frame 1
    player->addHitbox("idle_right", 1, 4, 1, 12, 7);
    player->addHitbox("idle_right", 1, 5, 7, 11, 11);
    player->addHitbox("idle_right", 1, 5, 11, 11, 15);

    // walk left
    // frame 0
    player->addHitbox("walk_left", 0, 3, 0, 11, 6);
    player->addHitbox("walk_left", 0, 4, 6, 10, 11);
    player->addHitbox("walk_left", 0, 4, 11, 10, 15);

    // frame 1
    player->addHitbox("walk_left", 1, 3, 0, 11, 6);
    player->addHitbox("walk_left", 1, 4, 6, 10, 11);
    player->addHitbox("walk_left", 1, 5, 11, 9, 15);

    // walk right
    // frame 0
    player->addHitbox("walk_right", 0, 4, 0, 12, 6);
    player->addHitbox("walk_right", 0, 5, 6, 11, 11);
    player->addHitbox("walk_right", 0, 5, 11, 11, 15);

    // frame 1
    player->addHitbox("walk_right", 1, 4, 0, 12, 6);
    player->addHitbox("walk_right", 1, 5, 6, 11, 11);
    player->addHitbox("walk_right", 1, 6, 11, 10, 15);

    // jump left
    // frame 0
    player->addHitbox("jump_left", 0, 3, 0, 11, 6);
    player->addHitbox("jump_left", 0, 4, 6, 10, 10);
    player->addHitbox("jump_left", 0, 4, 10, 10, 12);

    // jump right
    // frame 0
    player->addHitbox("jump_right", 0, 4, 0, 12, 6);
    player->addHitbox("jump_right", 0, 5, 6, 11, 10);
    player->addHitbox("jump_right", 0, 5, 10, 11, 12);

    // fall left
    // frame 0
    player->addHitbox("fall_left", 0, 3, 1, 11, 7);
    player->addHitbox("fall_left", 0, 4, 7, 10, 11);
    player->addHitbox("fall_left", 0, 4, 11, 10, 15);

    // fall right
    // frame 0
    player->addHitbox("fall_right", 0, 4, 1, 12, 7);
    player->addHitbox("fall_right", 0, 5, 7, 11, 11);
    player->addHitbox("fall_right", 0, 5, 11, 11, 15);

    // Set idle_right as starting animation
    player->setAnimation("idle_right");

    glGenVertexArrays(1, &Level::VAO);
    glGenBuffers(1, &Level::VBO);
    glGenBuffers(1, &Level::EBO);
    Level::hbShader = new Shader("shaders/hitboxes.vert", "shaders/hitboxes.frag");

    Global::level = new Level();

    int layer = Global::level->newLayer(14, 14);
    player->addCollisionLayer(layer);

    int carpet = Global::level->addTile("floor", 0, 0, 1, 1, false);
    int brick = Global::level->addTile("brick", 0, 0, 1, 1, true);
    Global::debug = Global::level->addTile("debug", 0, 0, 1, 1, true);

    Global::level->addHitbox(brick, 0, 0, 14, 14);

    Global::level->fillLayer(layer, brick, 0, 1, 3, 2);
    Global::level->fillLayer(layer, brick, 5, 3, 7, 4);

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
