#ifndef TYPES_H
#define TYPES_H

typedef struct {
    int width;
    int height;
    int nrc;
    unsigned int id;
    int rows;
    int cols;
} Texture;

typedef struct {
    int x;
    int y;
    int angle;
} Camera;

#endif
