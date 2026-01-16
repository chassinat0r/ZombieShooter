#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <engine/texturemgr.h>
#include <engine/fontmgr.h>

#include <engine/sprite.h>
#include <engine/animation.h>
#include <engine/keyboard.h>
#include <engine/level.h>

#include <stdio.h>
#include <math.h>

#include <global.h>
#include <constants.h>
#include <player.h>
#include <zombie.h>
#include <hotbar.h>
#include <util.h>
#include <bullet.h>

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
Hotbar *hotbar;

Camera camera = {0, 0, 0};

glm::vec2 mousePos = glm::vec2(0, 0);

float projectileAngle = 0.0f;
float realAngle = 0.0f;

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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    mousePos.x = xpos;
    mousePos.y = ypos;

    Inv_Item *selectedItem = hotbar->getSelectedItem();

    if (selectedItem->itemType == LD_RANGED) {
        std::pair<float,float> scrDimensions = getRenderWidthAndHeight();
        float scrMouseX = (mousePos.x - 0.5f*(float)Global::width) * (scrDimensions.first / (float)Global::width);
        float scrMouseY = (mousePos.y - 0.5f*(float)Global::height) * (scrDimensions.second / (float)Global::height);
        
        int playerDirection = player->getDirection();
        if ((playerDirection == 1 && scrMouseX <= 0)
        || (playerDirection == 2 && scrMouseX >= 0)) {
            projectileAngle = atan2(scrMouseX, (playerDirection == 1 ? scrMouseY : -scrMouseY)) + (playerDirection == 1 ? 0.5f*M_PI : -0.5f*M_PI);
            
            if (projectileAngle < -0.25f*M_PI) {
                projectileAngle = -0.25f*M_PI;
            } else if (projectileAngle > 0.25f*M_PI) {
                projectileAngle = 0.25f*M_PI;
            } else {
                realAngle = atan2(scrMouseX, -scrMouseY);
            }
        }

    }
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
    Global::width = w;
    Global::height = h;
    glViewport(0, 0, Global::width, Global::height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        Inv_Item *selectedItem = hotbar->getSelectedItem();

        if (selectedItem->itemType == LD_RANGED) {
            if (selectedItem->itemId == 1) {
                if (selectedItem->isInTimeout) {
                    printf("timeout\n");
                } else if (selectedItem->isReloading) {
                    printf("gun is reloading\n");
                } else if (selectedItem->loadedAmmo == 0) {
                    if (selectedItem->totalAmmo == 0) {
                        printf("no more ammo\n");
                    } else {
                        printf("press R to reload ammo\n");
                    }
                } else {
                    printf("shoot gun\n");
                    float startX = (player->getDirection() == 1) ? player->getX() - 10.0f : player->getX() + 10.0f;
                    new Bullet(startX, player->getY(), realAngle, 1.0f);
                    selectedItem->loadedAmmo--;
                    selectedItem->isInTimeout = true;
                    selectedItem->timeout = 0;
                }
            }
        }
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        printf("right button clicked\n");
    }
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

    if (Keyboard::isKeyPressed(window, GLFW_KEY_1)) {
        hotbar->selectItem(0);
    }
    if (Keyboard::isKeyReleased(window, GLFW_KEY_1)) {

    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_2)) {
        hotbar->selectItem(1);
    }
    if (Keyboard::isKeyReleased(window, GLFW_KEY_2)) {

    }

    if (Keyboard::isKeyPressed(window, GLFW_KEY_R)) {
        hotbar->reload();
    }
    if (Keyboard::isKeyReleased(window, GLFW_KEY_R)) {

    }
}

void update() {
    Sprite::updateAll();

    hotbar->update();

    camera.x = (int)player->getX();
    camera.y = (int)player->getY();

    running = !glfwWindowShouldClose(window);
}

void draw() {
    glClearColor(0.7f, 0.9f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Global::level->render(&camera);

    Sprite::drawAll(&camera);

    player->drawHealthBar();

    hotbar->draw();

    Inv_Item *selectedItem = hotbar->getSelectedItem();

    glm::vec2 reflection = glm::vec2(0, 0);

    if (player->getDirection() == 1) {
        reflection.x = 1;
    }

    if (selectedItem != nullptr) {
        switch (selectedItem->itemId) {
            case 0:
                TextureManager::setTex("knife", 0, 0, 1, 1);
                TextureManager::drawTex(std::floor(player->getX())+3.0f, std::floor(player->getY()), 0.6f, 0.3f, &camera, reflection);
                break;
            case 1:
                TextureManager::setTex("pistol", 0, 0, 1, 1, "left", "centre");
                TextureManager::drawTex(std::floor(player->getX())+2.0f, std::floor(player->getY())+1.0f, 0.4f, projectileAngle, &camera, reflection);
                break;
        }
    }

    // TextureManager::setTex("knife", 0, 0, 1, 1);
    // TextureManager::drawTex(0, 0, 3.0f, toRadians(90));
    glfwSwapBuffers(window);
}

int main() {
    Inv_Item knife;
    knife.itemId = 0;
    knife.itemType = MELEE;
    knife.isInTimeout = false;
    knife.timeout = 0;
    knife.timeoutDuration = 200;
    knife.hasDurability = true;
    knife.currentDurability = 70;
    knife.maxDurability = 100;

    Inv_Item pistol;
    pistol.itemId = 1;
    pistol.itemType = LD_RANGED;
    pistol.isInTimeout = false;
    pistol.timeout = 0;
    pistol.timeoutDuration = 800;
    pistol.doesReload = true;
    pistol.reloadTimePerAmmo = 1000;
    pistol.loadedAmmo = 5;
    pistol.maxLoadedAmmo = 8;
    pistol.totalAmmo = 60;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

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
    TextureManager::loadTex("assets/terrain.png", "terrain", 1, 3);
    TextureManager::loadTex("assets/ui/hotbar_slot.png", "hotbar_slot", 1, 1);
    TextureManager::loadTex("assets/ui/hotbar_selected.png", "hotbar_selected", 1, 1);

    TextureManager::loadTex("assets/knife.png", "knife", 1, 1);
    TextureManager::loadTex("assets/pistol.png", "pistol", 1, 1);
    TextureManager::loadTex("assets/bullet.png", "bullet", 1, 1);

    TextureManager::loadTex("assets/hero_arm.png", "hero_arm", 2, 2);

    FontManager::init();
    FontManager::loadFont("assets/fonts/arial.ttf", "arial24", 24);
    
    player = new Player(0, 55, 1.0f, true);
    player->setMaxHealth(25);
    player->setHealth(25);

    zombie = new Zombie(80, 55, 100, 100, 1.0f, true);
    zombie->setMaxHealth(10);
    zombie->setHealth(10);
    zombie->setTarget(player->getID());
    
    Inv_Item hotbarItems[2] = {knife, pistol};
    hotbar = new Hotbar(hotbarItems);

    glGenVertexArrays(1, &Level::VAO);
    glGenBuffers(1, &Level::VBO);
    glGenBuffers(1, &Level::EBO);
    Level::hbShader = new Shader("shaders/hitboxes.vert", "shaders/hitboxes.frag");

    Global::level = new Level();

    int layer = Global::level->newLayer(12, 12);
    player->addCollisionLayer(layer);
    zombie->addCollisionLayer(layer);

    int carpet = Global::level->addTile("floor", "carpet", 0, 0, 1, 1, false);
    int grass = Global::level->addTile("terrain", "ground", 0, 0, 1, 1, true);
    int dirt = Global::level->addTile("terrain", "ground", 0, 1, 1, 2, true);
    int brick = Global::level->addTile("terrain", "ground", 0, 2, 1, 3, true);

    Global::debug = Global::level->addTile("debug", "debug", 0, 0, 1, 1, true);

    Global::level->addHitbox(brick, 0, 0, 12, 12);
    Global::level->addHitbox(grass, 0, 0, 12, 12);
    Global::level->addHitbox(dirt, 0, 0, 12, 12);

    Global::level->fillLayer(layer, grass, 0, 2, 15, 3);
    Global::level->fillLayer(layer, brick, 3, 3, 6, 4);

    Global::level->fillLayer(layer, dirt, 0, 0, 15, 2);
    Global::level->fillLayer(layer, dirt, 15, 0, 18, 3);
    Global::level->fillLayer(layer, grass, 15, 3, 18, 4);

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
