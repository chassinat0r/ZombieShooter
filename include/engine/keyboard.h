#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <GLFW/glfw3.h>

#include <map>

/* Module for handling keyboard input
As GLFW doesn't seem to differentiate a key being released AFTER being
pressed versus a key simply not being pressed, we need to workaround this.
I do it by storing a map of any key that is checked if it is pressed
along with its boolean value (true for pressed, false for not).
When the key requested is pressed, it will set that key in the map to true
and return true.
When a key is checked if it is released, the function will first check if it 
was pressed before in the map and ONLY if that is the case return true.
*/

class Keyboard {
    public:
        // Functions for seeing if a key is pressed or released
        static bool isKeyPressed(GLFWwindow* window, int key);
        static bool isKeyReleased(GLFWwindow* window, int key);

        inline static std::map<int, bool> keys; // Store any requested key and their boolean state
};

#endif
