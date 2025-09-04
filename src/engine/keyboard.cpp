#include <engine/keyboard.h>

/* Keyboard::isKeyPressed
Description: Determine if a given key has been pressed. The press will
only happen once until the key is released.
Parameters:
- GLFWwindow *window: Necessary for using the glfwGetKey function
- int key: To check if that key is pressed it will use its integer value
Output: Boolean value, whether that key has been pressed
*/

bool Keyboard::isKeyPressed(GLFWwindow* window, int key) {
    if (glfwGetKey(window, key) == GLFW_PRESS) { // If keystate is pressed
        if (!keys[key]) { // If key is not already registered as pressed
            keys[key] = true; // Set it as such
            return true;
        }
    }
    return false; // If the key is not pressed or has already been recorded as pressed
}

/* Keyboard::isKeyReleased 
Description: Determine if a given key has been released after previously
being pressed.
Parameters: 
- GLFWwindow *window: Necessary for using the glfwGetKey function.
- int key: To check if that key is released it will use its integer value.
Output: Boolean value, whether that key has been released.
*/

bool Keyboard::isKeyReleased(GLFWwindow* window, int key) {
    if (glfwGetKey(window, key) == GLFW_RELEASE) { // If keystate is release (AKA not pressed)
        if (keys[key]) { // If key was previously pressed before this check
            keys[key] = false; // Set it as no longer pressed
            return true; // Return true
        }
    }
    return false; // If key was not pressed when last checked or is still pressed
}
