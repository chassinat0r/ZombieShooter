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
bool playerMoving = false;
int playerDir = 0;
// 0 forward, 1 backward, 2 left, 3 right
// int count = 0;

GLFWwindow* window;

Sprite *player;
Zombie *zombie;

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

    if (zombie->targetMovedFromDestination()) {
        zombie->getPathToTarget();
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
    TextureManager::loadTex("assets/player.png", "player", 4, 4);
    TextureManager::loadTex("assets/stone.png", "stone", 1, 1);
    TextureManager::loadTex("assets/floor.png", "floor", 1, 1);
    TextureManager::loadTex("assets/debug.png", "debug", 1, 1);

    TextureManager::loadTex("assets/zombie.png", "zombie", 4, 4);

    player = new Sprite(-48, 30, 1.0f, true);
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
    // frame 0
    player->addHitbox("front_still", "head", 0, 3, 0, 12, 9);
    player->addHitbox("front_still", "torso", 0, 3, 9, 12, 16);
    player->addHitbox("front_still", "arm", 0, 1, 9, 3, 16);
    player->addHitbox("front_still", "arm", 0, 12, 9, 14, 16);
    player->addHitbox("front_still", "leg", 0, 3, 16, 6, 24);
    player->addHitbox("front_still", "leg", 0, 9, 16, 12, 24);
    player->addHitbox("front_still", "crotch", 0, 6, 16, 9, 18);

    // frame 1
    player->addHitbox("front_still", "head", 1, 3, 1, 12, 10);
    player->addHitbox("front_still", "torso", 1, 3, 10, 12, 17);
    player->addHitbox("front_still", "arm", 1, 1, 10, 3, 17);
    player->addHitbox("front_still", "arm", 1, 12, 10, 14, 17);
    player->addHitbox("front_still", "leg", 1, 3, 17, 6, 24);
    player->addHitbox("front_still", "leg", 1, 9, 17, 12, 24);
    player->addHitbox("front_still", "crotch", 1, 6, 17, 9, 19);

    // back still
    // frame 0
    player->addHitbox("back_still", "head", 0, 3, 0, 12, 9);
    player->addHitbox("back_still", "torso", 0, 3, 9, 12, 16);
    player->addHitbox("back_still", "arm", 0, 1, 9, 3, 16);
    player->addHitbox("back_still", "arm", 0, 12, 9, 14, 16);
    player->addHitbox("back_still", "leg", 0, 3, 16, 6, 24);
    player->addHitbox("back_still", "leg", 0, 9, 16, 12, 24);
    player->addHitbox("back_still", "crotch", 0, 6, 16, 9, 18);

    // frame 1
    player->addHitbox("back_still", "head", 1, 3, 1, 12, 10);
    player->addHitbox("back_still", "torso", 1, 3, 10, 12, 17);
    player->addHitbox("back_still", "arm", 1, 1, 10, 3, 17);
    player->addHitbox("back_still", "arm", 1, 12, 10, 14, 17);
    player->addHitbox("back_still", "leg", 1, 3, 17, 6, 24);
    player->addHitbox("back_still", "leg", 1, 9, 17, 12, 24);
    player->addHitbox("back_still", "crotch", 1, 6, 17, 9, 19);

    // right still
    // frame 0
    player->addHitbox("right_still", "head", 0, 3, 0, 12, 9);
    player->addHitbox("right_still", "torso", 0, 6, 9, 10, 16);
    player->addHitbox("right_still", "arm", 0, 8, 10, 9, 15);
    player->addHitbox("right_still", "leg", 0, 6, 16, 10, 24);

    // frame 1
    player->addHitbox("right_still", "head", 1, 3, 1, 12, 10);
    player->addHitbox("right_still", "torso", 1, 6, 10, 10, 17);
    player->addHitbox("right_still", "arm", 1, 8, 11, 9, 16);
    player->addHitbox("right_still", "leg", 1, 6, 17, 10, 24);

    // left still
    // frame 0
    player->addHitbox("left_still", "head", 0, 3, 0, 12, 9);
    player->addHitbox("left_still", "torso", 0, 5, 9, 9, 16);
    player->addHitbox("left_still", "arm", 0, 6, 10, 7, 15);
    player->addHitbox("left_still", "leg", 0, 5, 16, 9, 24);

    // frame 1
    player->addHitbox("left_still", "head", 1, 3, 1, 12, 10);
    player->addHitbox("left_still", "torso", 1, 5, 10, 9, 17);
    player->addHitbox("left_still", "arm", 1, 6, 11, 7, 16);
    player->addHitbox("left_still", "leg", 1, 5, 17, 9, 24);

    // front walk
    // frame 0
    player->addHitbox("front_walk", "head", 0, 3, 1, 12, 10);
    player->addHitbox("front_walk", "torso", 0, 3, 10, 12, 16);
    player->addHitbox("front_walk", "arm", 0, 1, 10, 3, 16);
    player->addHitbox("front_walk", "arm", 0, 12, 10, 14, 14);
    player->addHitbox("front_walk", "leg", 0, 3, 16, 6, 22);
    player->addHitbox("front_walk", "leg", 0, 9, 16, 12, 24);
    player->addHitbox("front_walk", "crotch", 0, 6, 16, 9, 18);

    // frame 1
    player->addHitbox("front_walk", "head", 1, 3, 1, 12, 10);
    player->addHitbox("front_walk", "torso", 1, 3, 10, 12, 16);
    player->addHitbox("front_walk", "arm", 1, 1, 10, 3, 14);
    player->addHitbox("front_walk", "arm", 1, 12, 10, 14, 16);
    player->addHitbox("front_walk", "leg", 1, 3, 16, 6, 24);
    player->addHitbox("front_walk", "leg", 1, 9, 16, 12, 22);
    player->addHitbox("front_walk", "crotch", 1, 6, 16, 9, 18);

    // back walk
    // frame 0
    player->addHitbox("back_walk", "head", 0, 3, 1, 12, 10);
    player->addHitbox("back_walk", "torso", 0, 3, 10, 12, 16);
    player->addHitbox("back_walk", "arm", 0, 1, 10, 3, 16);
    player->addHitbox("back_walk", "arm", 0, 12, 10, 14, 14);
    player->addHitbox("back_walk", "leg", 0, 3, 16, 6, 22);
    player->addHitbox("back_walk", "leg", 0, 9, 16, 12, 24);
    player->addHitbox("back_walk", "crotch", 0, 6, 16, 9, 18);

    // frame 1
    player->addHitbox("back_walk", "head", 1, 3, 1, 12, 10);
    player->addHitbox("back_walk", "torso", 1, 3, 10, 12, 16);
    player->addHitbox("back_walk", "arm", 1, 1, 10, 3, 14);
    player->addHitbox("back_walk", "arm", 1, 12, 10, 14, 16);
    player->addHitbox("back_walk", "leg", 1, 3, 16, 6, 24);
    player->addHitbox("back_walk", "leg", 1, 9, 16, 12, 22);
    player->addHitbox("back_walk", "crotch", 1, 6, 16, 9, 18);

    // left walk
    // frame 0
    player->addHitbox("left_walk", "head", 0, 3, 1, 12, 10);
    player->addHitbox("left_walk", "torso", 0, 5, 10, 9, 16);
    player->addHitbox("left_walk", "arm", 0, 3, 13, 5, 15);
    player->addHitbox("left_walk", "leg", 0, 5, 16, 10, 24);

    // frame 1
    player->addHitbox("left_walk", "head", 1, 3, 1, 12, 10);
    player->addHitbox("left_walk", "torso", 1, 5, 10, 9, 17);
    player->addHitbox("left_walk", "arm", 1, 6, 11, 7, 16);
    player->addHitbox("left_walk", "leg", 1, 5, 17, 9, 24);

    // frame 2
    player->addHitbox("left_walk", "head", 2, 3, 1, 12, 10);
    player->addHitbox("left_walk", "torso", 2, 5, 10, 9, 17);
    player->addHitbox("left_walk", "arm", 2, 7, 12, 10, 15);
    player->addHitbox("left_walk", "leg", 2, 4, 16, 9, 24);

    // frame 3
    player->addHitbox("left_walk", "head", 3, 3, 1, 12, 10);
    player->addHitbox("left_walk", "torso", 3, 5, 10, 9, 17);
    player->addHitbox("left_walk", "arm", 3, 6, 11, 7, 16);
    player->addHitbox("left_walk", "leg", 3, 5, 17, 9, 24);

    // right walk
    // frame 0
    player->addHitbox("right_walk", "head", 0, 3, 1, 12, 10);
    player->addHitbox("right_walk", "torso", 0, 6, 10, 10, 16);
    player->addHitbox("right_walk", "arm", 0, 10, 13, 12, 15);
    player->addHitbox("right_walk", "leg", 0, 5, 16, 10, 24);

    // frame 1
    player->addHitbox("right_walk", "head", 1, 3, 1, 12, 10);
    player->addHitbox("right_walk", "torso", 1, 6, 10, 10, 17);
    player->addHitbox("right_walk", "arm", 1, 8, 11, 9, 16);
    player->addHitbox("right_walk", "leg", 1, 6, 17, 10, 24);

    // frame 2
    player->addHitbox("right_walk", "head", 2, 3, 1, 12, 10);
    player->addHitbox("right_walk", "torso", 2, 6, 10, 10, 16);
    player->addHitbox("right_walk", "arm", 2, 5, 12, 7, 15);
    player->addHitbox("right_walk", "leg", 2, 6, 16, 11, 24);

    // frame 3
    player->addHitbox("right_walk", "head", 3, 3, 1, 12, 10);
    player->addHitbox("right_walk", "torso", 3, 6, 10, 10, 17);
    player->addHitbox("right_walk", "arm", 3, 8, 11, 9, 16);
    player->addHitbox("right_walk", "leg", 3, 6, 17, 10, 24);

    player->setAnimation("front_still");

    player->allowHitboxCollision("leg", "leg");
    player->allowHitboxCollision("head", "head");
    player->allowHitboxCollision("torso", "torso");
    player->allowHitboxCollision("arm", "torso");
    player->allowHitboxCollision("torso", "arm");

    zombie = new Zombie(30, 30, 1.0f, true);
    zombie->setTarget(player->getID());
    
    Animation zombieFrontStill("zombie_front_still");
    zombieFrontStill.addFrame("zombie", 0, 0, 1, 1, 700);
    zombieFrontStill.addFrame("zombie", 0, 1, 1, 2, 700);
    
    zombie->addAnimation("zombie_front_still", zombieFrontStill);

    // frame 0
    zombie->addHitbox("zombie_front_still", "head", 0, 3, 1, 12, 10);
    zombie->addHitbox("zombie_front_still", "torso", 0, 3, 10, 12, 17);
    zombie->addHitbox("zombie_front_still", "arm", 0, 1, 11, 3, 17);
    zombie->addHitbox("zombie_front_still", "arm", 0, 12, 11, 14, 18);
    zombie->addHitbox("zombie_front_still", "leg", 0, 3, 17, 6, 24);
    zombie->addHitbox("zombie_front_still", "leg", 0, 9, 17, 12, 24);
    zombie->addHitbox("zombie_front_still", "crotch", 0, 6, 17, 9, 19);

    // frame 1
    zombie->addHitbox("zombie_front_still", "head", 1, 3, 2, 12, 11);
    zombie->addHitbox("zombie_front_still", "torso", 1, 3, 11, 7, 17);
    zombie->addHitbox("zombie_front_still", "torso", 1, 7, 11, 12, 18);
    zombie->addHitbox("zombie_front_still", "arm", 1, 1, 11, 3, 17);
    zombie->addHitbox("zombie_front_still", "arm", 1, 12, 12, 14, 19);
    zombie->addHitbox("zombie_front_still", "leg", 1, 3, 17, 6, 24);
    zombie->addHitbox("zombie_front_still", "leg", 1, 9, 18, 12, 24);
    zombie->addHitbox("zombie_front_still", "crotch", 1, 6, 18, 9, 20);

    zombie->setAnimation("zombie_front_still");

    zombie->allowHitboxCollision("leg", "leg");
    zombie->allowHitboxCollision("head", "head");
    zombie->allowHitboxCollision("torso", "torso");
    zombie->allowHitboxCollision("arm", "torso");
    zombie->allowHitboxCollision("torso", "arm");

    glGenVertexArrays(1, &Level::VAO);
    glGenBuffers(1, &Level::VBO);
    glGenBuffers(1, &Level::EBO);
    Level::hbShader = new Shader("shaders/hitboxes.vert", "shaders/hitboxes.frag");

    Global::level = new Level();

    int floor = Global::level->newLayer(16, 16);
    int layer = Global::level->newLayer(16, 12);
    player->addCollisionLayer(layer);
    zombie->addCollisionLayer(layer);

    int carpet = Global::level->addTile("floor", 0, 0, 1, 1, false);
    int stone = Global::level->addTile("stone", 0, 0, 1, 1, true);
    Global::debug = Global::level->addTile("debug", 0, 0, 1, 1, true);

    Global::level->addHitbox(stone, 0, 0, 16, 8);

    Global::level->fillLayer(layer, stone, -10, 10, 10, 9);
    Global::level->fillLayer(layer, stone, -10, -9, 10, -10);
    Global::level->fillLayer(layer, stone, -10, 10, -9, -10);
    Global::level->fillLayer(layer, stone, 9, 10, 10, -10);

    Global::level->fillLayer(floor, carpet, -10, -8, 10, 8);

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
