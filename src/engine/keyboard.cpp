#include <engine/keyboard.h>

bool Keyboard::isKeyPressed(GLFWwindow* window, int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) {
        if (!keys[key]) {
            keys[key] = true;
            return true;
        }
    }
    return false;
}

bool Keyboard::isKeyReleased(GLFWwindow* window, int key) {
    if (glfwGetKey(window, key) == GLFW_RELEASE) {
        if (keys[key]) {
            keys[key] = false;
            return true;
        }
    }
    return false;
}
