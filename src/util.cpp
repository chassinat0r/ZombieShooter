#include <util.h>

#include <global.h>

void mergeArrays(const float *vertices, int vertices_length,  int vert_stride, const float *texture_coords, int tex_length, int tex_stride, float *final_arr, int final_length) {
	int final_stride = vert_stride + tex_stride; 
	int vertices_count = final_length / final_stride;

	int vert_pos = 0;
	int tex_pos = 0;
	int final_pos = 0;

	while (final_pos < final_length) {
		int vert_end = vert_pos + vert_stride;
		int tex_end = tex_pos + tex_stride;
		while (vert_pos < vert_end) {
			final_arr[final_pos] = vertices[vert_pos];
			final_pos++;
			vert_pos++;
		}
		while (tex_pos < tex_end) {
			final_arr[final_pos] = texture_coords[tex_pos];
			final_pos++;
			tex_pos++;
		}
	}
}

bool doHitboxesCollide(Rect_F hb1, Rect_F hb2) {
	float h1x1 = std::min(hb1.x1, hb1.x2);
	float h1x2 = std::max(hb1.x1, hb1.x2);
	float h1y1 = std::min(hb1.y1, hb1.y2);
	float h1y2 = std::max(hb1.y1, hb1.y2);

	float h2x1 = std::min(hb2.x1, hb2.x2);
	float h2x2 = std::max(hb2.x1, hb2.x2);
	float h2y1 = std::min(hb2.y1, hb2.y2);
	float h2y2 = std::max(hb2.y1, hb2.y2);

	if (std::min(h1x2, h2x2) >= std::max(h1x1, h2x1) && std::min(h1y2, h2y2) >= std::max(h1y1, h2y1)) {
		return true;
	}
	
	return false;
}

bool doObjectsCollide(CollidableObject obj1, CollidableObject obj2) {
	std::vector<Rect_F> obj1Hitboxes = getRealHitboxes(obj1.hitboxes, obj1.x, obj1.y, obj1.width, obj1.height, obj1.scale, obj1.anchorX, obj1.anchorY);
	std::vector<Rect_F> obj2Hitboxes = getRealHitboxes(obj2.hitboxes, obj2.x, obj2.y, obj2.width, obj2.height, obj2.scale, obj2.anchorX, obj2.anchorY);

	std::vector<Rect_F> obj1Lines = getCollisionLines(obj1Hitboxes);
	std::vector<Rect_F> obj2Lines = getCollisionLines(obj2Hitboxes);

	for (Rect_F l1 : obj1Lines) {
		float l1arr[2][2] = {{l1.x1, l1.y1}, {l1.x2, l1.y2}};
		for (Rect_F l2 : obj2Lines) {
			float l2arr[2][2] = {{l2.x1, l2.y1}, {l2.x2, l2.y2}};
			if (doLinesIntersect(l1arr, l2arr)) {
				return true;
			}
		}
	}

	return false;
}

Rect_F getRealHitbox(Rect objHitbox, float x, float y, int width, int height, float scale, std::string anchorX, std::string anchorY) {
	Rect_F hitbox;
	
	float x1 = std::min(objHitbox.x1, objHitbox.x2);
	float x2 = std::max(objHitbox.x1, objHitbox.x2);
	float y1 = std::min(objHitbox.y1, objHitbox.y2);
	float y2 = std::max(objHitbox.y1, objHitbox.y2);

	if (anchorX == "left" || anchorX == "LEFT") {
		hitbox.x1 = x + x1*scale;
		hitbox.x2 = x + x2*scale;
	} else if (anchorX == "right" || anchorX == "RIGHT") {
		hitbox.x1 = x - width*scale + x1*scale;
		hitbox.x2 = x - width*scale + x2*scale;
	} else {
		hitbox.x1 = x - 0.5f*width*scale + x1*scale;
		hitbox.x2 = x - 0.5f*width*scale + x2*scale;
	}

	if (anchorY == "bottom" || anchorY == "BOTTOM") {
		hitbox.y1 = y + y1*scale;
		hitbox.y2 = y + y2*scale;
	} else if (anchorY == "top" || anchorY == "TOP") {
		hitbox.y1 = y - height*scale + y1*scale;
		hitbox.y2 = y - height*scale + y2*scale;
	} else {
		hitbox.y1 = y + 0.5f*height*scale - y2*scale;
		hitbox.y2 = y + 0.5f*height*scale - y1*scale;
	}

	return hitbox;
}

std::vector<Rect_F> getCollisionLines(std::vector<Rect_F> realHitboxes) {
	std::vector<Rect_F> collisionLines;

	for (Rect_F rhb : realHitboxes) {
		Rect_F l1 = { rhb.x1, rhb.y1, rhb.x1, rhb.y2 };
		Rect_F l2 = { rhb.x1, rhb.y1, rhb.x2, rhb.y1 };
		Rect_F l3 = { rhb.x1, rhb.y2, rhb.x2, rhb.y2 };
		Rect_F l4 = { rhb.x2, rhb.y1, rhb.x2, rhb.y2 };

		collisionLines.push_back(l1);
		collisionLines.push_back(l2);
		collisionLines.push_back(l3);
		collisionLines.push_back(l4);
	}

	return collisionLines;
}

std::vector<Rect_F> getRealHitboxes(std::vector<Rect> objHitboxes, float x, float y, int width, int height, float scale, std::string anchorX, std::string anchorY) {
	std::vector<Rect_F> realHitboxes;

	for (Rect hb : objHitboxes) {

		realHitboxes.push_back(getRealHitbox(hb, x, y, width, height, scale, anchorX, anchorY));
	}

	return realHitboxes;
}

std::vector<Rect_F> getHitboxVector(std::map<std::string,std::vector<Rect_F>> hitboxes) {
	std::vector<Rect_F> hbVec;
	for (auto it : hitboxes) {
		for (Rect_F hb : it.second) {
			hbVec.push_back(hb);
		}
	}
	return hbVec;
}

std::pair<float, float> getRenderWidthAndHeight() {
	float h = (Global::height < 700) ? DEF_HEIGHT : DEF_HEIGHT * (Global::height / 700.0f);
    float w = (h / (float)Global::height) * Global::width; 
    
	return { w, h };
}

double toDegrees(double radians) {
	return radians * (180.0f/M_PI);
}

double toRadians(double degrees) {
	return degrees * (M_PI/180.0f);
}

bool onSegment(float p[2], float q[2], float r[2]) {
	return (q[0] <= std::max(p[0], r[0]) && 
            q[0] >= std::min(p[0], r[0]) &&
            q[1] <= std::max(p[1], r[1]) && 
            q[1] >= std::min(p[1], r[1]));
}

int orientation(float p[2], float q[2], float r[2]) {
	float val = (q[1] - p[1]) * (r[0] - q[0]) -
				(q[0] - p[0]) * (r[1] - q[1]);
	
	if (val == 0) { return 0; }

	return (val > 0) ? 1 : 2;
}

bool doLinesIntersect(float l1[2][2], float l2[2][2]) {
	int o1 = orientation(l1[0], l1[1], l2[0]);
	int o2 = orientation(l1[0], l1[1], l2[1]);
	int o3 = orientation(l2[0], l2[1], l1[0]);
	int o4 = orientation(l2[0], l2[1], l1[1]);

	if (o1 != o2 && o3 != o4) { return true; }

	if (o1 == 0 && onSegment(l1[0], l2[0], l1[1])) { return true; }

	if (o2 == 0 && onSegment(l1[0], l2[1], l1[1])) { return true; }

	if (o3 == 0 && onSegment(l2[0], l1[0], l2[1])) { return true; }
	
	if (o4 == 0 && onSegment(l2[0], l1[1], l2[1])) { return true; }

	return false;
}
