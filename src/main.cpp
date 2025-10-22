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
#include <player.h>
#include <zombie.h>

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

Player *player;
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

    if (Keyboard::isKeyPressed(window, GLFW_KEY_A)) {
        player->setDirection(1);
        player->startMoving();
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_D)) {
        player->setDirection(2);
        player->startMoving();
    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_SPACE)) {
        if (player->isGrounded()) {
            player->jump();
        }
    }
    if (Keyboard::isKeyReleased(window, GLFW_KEY_SPACE)) {

    }

    if (Keyboard::isKeyReleased(window, GLFW_KEY_A)) {
        if (player->getDirection() == 1) {
            player->stopMoving();
        }
    }

    if (Keyboard::isKeyReleased(window, GLFW_KEY_D)) {
        if (player->getDirection() == 2) {
            player->stopMoving();
        }
    }
}

void update() {
    player->update();
    zombie->update();

    if (player->isCollidingWith(*zombie) && !zombie->isInCooldown()) {
        player->removeHealth();
        zombie->registerAttack();
    }

    camera.x = (int)player->getX();
    camera.y = (int)player->getY();

    running = !glfwWindowShouldClose(window);
}

void draw() {
    glClearColor(0.55f, 0.01f, 0.55f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Global::level->render(&camera);

    std::vector<Sprite*> sorted = Sprite::sprites;

    quickSort(&sorted, 0, sorted.size()-1);
    
    for (Sprite *s : sorted) {
        s->draw(&camera);
    }

    player->drawHealthBar();

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
    TextureManager::loadTex("assets/zombie.png", "zombie", 2, 4);
    TextureManager::loadTex("assets/ui/health-bar.png", "health-bar", 1, 1);
    TextureManager::loadTex("assets/ui/health-icon.png", "health-icon", 1, 1);
    TextureManager::loadTex("assets/ui/health-states.png", "health-states", 1, 2);

    player = new Player(0, 55, 100, 100, 1.0f, true);
    zombie = new Zombie(20, 55, 100, 100, 1.0f, true);
    zombie->setTarget(player->getID());
    
    glGenVertexArrays(1, &Level::VAO);
    glGenBuffers(1, &Level::VBO);
    glGenBuffers(1, &Level::EBO);
    Level::hbShader = new Shader("shaders/hitboxes.vert", "shaders/hitboxes.frag");

    Global::level = new Level();

    int layer = Global::level->newLayer(12, 12);
    player->addCollisionLayer(layer);
    zombie->addCollisionLayer(layer);

    int carpet = Global::level->addTile("floor", 0, 0, 1, 1, false);
    int brick = Global::level->addTile("brick", 0, 0, 1, 1, true);
    Global::debug = Global::level->addTile("debug", 0, 0, 1, 1, true);

    Global::level->addHitbox(brick, 0, 0, 12, 12);

    Global::level->fillLayer(layer, brick, 0, 1, 10, 2);

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
