#ifndef UTIL_H
#define UTIL_H

#include <engine/types.h>

#include <vector>
#include <map>
#include <string>

void mergeArrays(const float *vertices, int vertices_length,  int vert_stride, const float *texture_coords, int tex_length, int tex_stride, float *final_arr, int final_length);
bool doHitboxesCollide(Rect_F hb1, Rect_F hb2);
bool doObjectsCollide(CollidableObject obj1, CollidableObject obj2);
Rect_F getRealHitbox(Rect objHitbox, float x, float y, int width, int height, float scale, std::string anchorX = "centre", std::string anchorY = "centre");
std::vector<Rect_F> getRealHitboxes(std::vector<Rect> objHitboxes, float x, float y, int width, int height, float scale, std::string anchorX = "centre", std::string anchorY = "centre");
std::vector<Rect_F> getHitboxVector(std::map<std::string,std::vector<Rect_F>> hitboxes);
std::pair<float, float> getRenderWidthAndHeight();
double toDegrees(double radians);
double toRadians(double degrees);
bool doLinesIntersect(float l1[2][2], float l2[2][2]);

#endif
