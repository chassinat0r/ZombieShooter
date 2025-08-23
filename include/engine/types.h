#ifndef TYPES_H
#define TYPES_H

#include <string>

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
    float angle;
} Camera;

typedef struct {
    std::string textureName; // name of texture in texture manager
    int r1; // starting row
    int c1; // starting column
    int r2; // ending row
    int c2; // ending column
    float duration; // in milliseconds
} Frame;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
} Rect;

typedef struct {
    float x1;
    float y1;
    float x2;
    float y2;
} Rect_F;

typedef struct {
    int x;
    int y;
} Pos_I;

typedef struct {
    float x;
    float y;
} Pos_F;

typedef struct {
    std::string textureName;
    int r1;
    int c1;
    int r2; 
    int c2;
    bool solid;
} Tile;

#endif
