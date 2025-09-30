#include <engine/animation.h>
#include <global.h>

Animation::Animation(std::string name) {
    this->name = name;
}

Animation::Animation() {

}

/* Animation:addFrame
Description: Add a frame to the animation 
Parameters:
- std::string textureName: The name of the texture the image will be 
sourced from
- int r1, c1, r2, and c2: The rows and columns on the texture
the image starts and ends at.
*/

void Animation::addFrame(std::string textureName, int r1, int c1, int r2, int c2, float duration) {
    frames.push_back({ textureName, r1, c1, r2, c2, duration }); // Push structure to the frames vector
}

/* Animation::getNumberOfFrames
Description: Get the number of frames in the animation.
Output: The length of the frames vector.
*/

int Animation::getNumberOfFrames() {
    return frames.size();
}

/* Animation::getFrame
Description: Get the Frame object at a given index.
Parameters:
- int index: The index of the Frame you want to return.
Output: Frame object corresponding to the index.
*/

Frame Animation::getFrame(int index) {
    if (index < frames.size()) { // Check if the index is in range
        return frames[index]; // Return the corresponding frame
    }

    return Frame(); // Return empty Frame if the index is out of range
}

/* Animation::addHitbox
Description: Add a hitbox for a frame to be checked for collision with other
solid sprites.
Parameters:
- int frame: Index of the frame to apply it on.
- int x1, y1, x2, y2: The start and end co-ordinates of the hitbox on the frame
image.
*/

void Animation::addHitbox(int frame, int x1, int y1, int x2, int y2) {
    std::vector<Rect> frameHitboxes; // Create an empty map for hitboxes of the frame
    if (hitboxes.count(frame) > 0) { // If previous hitboxes for that frame already exist
        frameHitboxes = hitboxes[frame]; // Import them all into the map so they aren't overwritten
    }

    frameHitboxes.push_back({x1, y1, x2, y2}); // Push the new hitbox to the vector

    hitboxes[frame] = frameHitboxes; // Write the new frame hitboxes map
}

/* Animation::getHitboxes
Description: Get a vector of all hitboxes of a given name and frame.
Parameters:
- std::string hbName: The name of the hitbox type you want.
- int frame: The frame index of the hitboxes.
Output: A vector of all hitboxes with the given name for the given frame.
*/

std::vector<Rect> Animation::getHitboxes(int frame) {
    if (hitboxes.count(frame) > 0) { // If there exist hitboxes for the given frame
        std::vector<Rect> frameHitboxes = hitboxes.at(frame); // Get a map of all hitboxes by their name at the given frame
        return frameHitboxes;
    }

    return std::vector<Rect>(); // If there aren't hitboxes for the given frame or name, return empty vector
}
