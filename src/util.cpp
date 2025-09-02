#include <util.h>

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

	if (std::min(h1x2, h2x2) > std::max(h1x1, h2x1) && std::min(h1y2, h2y2) > std::max(h1y1, h2y1)) {
		return true;
	}
	
	return false;
}

bool doObjectsCollide(std::vector<Rect_F> obj1, std::vector<Rect_F> obj2) {
	for (Rect_F hb1 : obj1) {
		for (Rect_F hb2 : obj2) {
			if (doHitboxesCollide(hb1, hb2)) {
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

std::vector<Rect_F> getRealHitboxes(std::vector<Rect> objHitboxes, float x, float y, int width, int height, float scale, std::string anchorX, std::string anchorY) {
	std::vector<Rect_F> realHitboxes;

	for (Rect hb : objHitboxes) {
		realHitboxes.push_back(getRealHitbox(hb, x, y, width, height, scale, anchorX, anchorY));
	}

	return realHitboxes;
}
