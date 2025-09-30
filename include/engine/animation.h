#ifndef ANIMATION_H
#define ANIMATION_H

#include <engine/types.h>

#include <string>
#include <vector>
#include <map>

/* Animation engine 
This module handles the animations of different sprites.
An animation is defined as an order of images which are selected from
a wider texture by the row and column they start and end at.
These images are known as frames and have a set time they run for until
the animation engine moves on to the next.
Animations have hitboxes which are defined by their start and end x and y
position in the image. Hitboxes are given a name so they can be set
to only collide with hitboxes of specific names, e.g. arms collide with torso
and arms but not legs.
*/

class Animation {
    public:
        // Constructor functions
        Animation(std::string name);
        Animation();

        // Adder functions
        void addFrame(std::string textureName, int r1, int c1, int r2, int c2, float duration);
        void addHitbox(int frame, int x1, int y1, int x2, int y2);
        std::vector<Rect> getHitboxes(int frame);

        // Getter functions
        int getNumberOfFrames();
        Frame getFrame(int index);

    private:
        std::string name; // Name of the animation
        std::vector<Frame> frames; // List of frames

        std::map<int, std::vector<Rect>> hitboxes; // Map of hitboxes for every frame
};

#endif
