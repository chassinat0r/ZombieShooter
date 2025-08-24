#ifndef UTIL_H
#define UTIL_H

#include <engine/types.h>

#include <vector>

void mergeArrays(const float *vertices, int vertices_length,  int vert_stride, const float *texture_coords, int tex_length, int tex_stride, float *final_arr, int final_length);
bool doObjectsCollide(std::vector<Rect_F> obj1, std::vector<Rect_F> obj2);
std::vector<Rect_F> getRealHitboxes(std::vector<Rect> objHitboxes, float x, float y, int width, int height, float scale, std::string anchorX = "centre", std::string anchorY = "centre");

#endif
