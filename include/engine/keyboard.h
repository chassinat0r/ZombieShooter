#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>

#include <map>

class Keyboard {
    public:
        static bool isKeyPressed(GLFWwindow* window, int key);
        static bool isKeyReleased(GLFWwindow* window, int key);

        inline static std::map<int, bool> keys;
};

#endif
