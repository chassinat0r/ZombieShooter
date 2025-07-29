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

GLFWwindow* window;

Sprite *jumpingMan;

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
}

void update() {
    running = !glfwWindowShouldClose(window);
}

void draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Camera camera;
    camera.x = 0;
    camera.y = 0;
    camera.angle = 0;

    jumpingMan->draw(camera, 3.0f);
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
    TextureManager::loadTex("assets/jumpingman.png", "jumpingman", 2, 2);
    // TextureManager::loadTex("assets/test.png", "test", 2, 3);

    jumpingMan = new Sprite(0, 0);
    Animation animation("jumping");
    animation.addFrame("jumpingman", 0, 0, 1, 1, 1000);
    animation.addFrame("jumpingman", 0, 1, 1, 2, 300);
    animation.addFrame("jumpingman", 1, 0, 2, 1, 300);
    animation.addFrame("jumpingman", 1, 1, 2, 2, 300);

    // animation.addFrame("test", 0, 0, 1, 1, 500);
    // animation.addFrame("test", 0, 1, 1, 2, 500);
    // animation.addFrame("test", 1, 0, 2, 1, 500);
    jumpingMan->addAnimation("jumping", animation);
    jumpingMan->setAnimation("jumping");

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
