#include "diamond_square.h"

#include <algorithm>

using std::min;
using std::max;

bool SaveImage(const string &filename, const std::vector<GLfloat> &data, size_t w, size_t h) {
	std::vector<BYTE> pixels;
	pixels.resize(3 * w * h);

	for (size_t y = 0; y < h; y++) {
		for (size_t x = 0; x < w; x++) {
			for (size_t i = 0; i < 3; i++) {
				pixels[3 * (y * w + x) + i] = data[3 * (y * w + x) + i];
			}
		}
	}
	FIBITMAP* Image = FreeImage_ConvertFromRawBits(pixels.data(), w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	return FreeImage_Save(FIF_PNG, Image, filename.c_str(), 0);
}

inline size_t Index(size_t x, size_t y, size_t w) {
	return x + y * w;
}

template<typename T>
inline typename T Sample(const vector<typename T> &hm, size_t x, size_t y, size_t w) {
	return hm[Index(x, y, w)];
}

void SquareStep(vector<GLfloat> &hm, size_t x, size_t y, size_t r, size_t w, float s) {
	hm[Index(x - r, y, w)] = (Sample(hm, x - r, y - r, w) + Sample(hm, x - r, y + r, w)) / 2.0f + RandNegFloat() * s;
	hm[Index(x + r, y, w)] = (Sample(hm, x + r, y - r, w) + Sample(hm, x + r, y + r, w)) / 2.0f + RandNegFloat() * s;

	hm[Index(x, y - r, w)] = (Sample(hm, x - r, y - r, w) + Sample(hm, x + r, y - r, w)) / 2.0f + RandNegFloat() * s;
	hm[Index(x, y + r, w)] = (Sample(hm, x - r, y + r, w) + Sample(hm, x + r, y + r, w)) / 2.0f + RandNegFloat() * s;
}

void DiamondStep(vector<GLfloat> &hm, size_t x, size_t y, size_t r, size_t w, float s) {
	float accum = Sample(hm, x , y - r, w);
	accum      += Sample(hm, x , y + r, w);
	accum      += Sample(hm, x - r, y , w);
	accum      += Sample(hm, x + r, y , w);

	accum *= 0.25;

	hm[Index(x, y, w)] = accum + RandNegFloat() * s;
}

void PowHeightMap(float p, vector<GLfloat> &hm, size_t cols, size_t rows) {
	for (size_t y = 0; y < rows; y++) {
		for (size_t x = 0; x < cols; x++) {
			hm[Index(x, y, cols)] = pow(hm[Index(x, y, cols)], p);
		}
	}
}

GLuint DiamondSquare(std::vector<GLfloat> &position_vec, size_t cols, size_t rows, float size) {
	vector<float> heightmap;
	heightmap.resize(rows * cols);

	float jitter = 1;
	Init(heightmap, cols, rows, jitter);

	float norm = jitter;
	for (size_t radius = rows >> 1; radius > 0; radius >>= 1) {
		jitter *= 0.5f;

		size_t step = radius << 1;
		for (size_t y = radius; y < rows - radius; y += step) {
			for (size_t x = radius; x < rows - radius; x += step) {
				SquareStep(heightmap, x, y, radius, cols, jitter);
			}
		}
		
		for (size_t y = radius; y < rows - radius; y += step) {
			for (size_t x = radius; x < rows - radius; x += step) {
				DiamondStep(heightmap, x, y, radius, cols, jitter);
			}
		}
		norm += jitter;
	}

	float min_v = heightmap[Index(0, 0, cols)], max_v = heightmap[Index(0, 0, cols)];
	for (size_t y = 0; y < rows; y++) {
		for (size_t x = 1; x < cols; x++) {
			min_v = min(heightmap[Index(x, y, cols)], min_v);
			max_v = max(heightmap[Index(x, y, cols)], max_v);
		}
	}
	
	for (size_t y = 0; y < rows; y++) {
		for (size_t x = 0; x < cols; x++) {
			heightmap[Index(x, y, cols)] -= min_v;
			heightmap[Index(x, y, cols)] /= max_v - min_v;
		}
	}

	PowHeightMap(2, heightmap, cols, rows);

	std::vector<GLubyte> pixels;
	for (size_t z = 0; z < rows; z++) {
		for (size_t x = 0; x < cols; x++) {
			position_vec[3 * (z * cols + x) + 1] = heightmap[z * cols + x] * 15.0f;
		}
	}
	
	for (size_t y = 0; y < rows; y++) {
		for (size_t x = 0; x < cols; x++) {
			heightmap[Index(x, y, cols)] *= 255;
			pixels.push_back(heightmap[Index(x, y, cols)]);
			pixels.push_back(heightmap[Index(x, y, cols)]);
			pixels.push_back(heightmap[Index(x, y, cols)]);
			pixels.push_back(1);
		}
	}
	GLuint hm = TextureFromPixelsNoMipMap(pixels.data(), rows, cols);
	return hm;
}

void Init(vector<GLfloat> &height_map, size_t cols, size_t rows, float h) {
	height_map[0]                 = RandFloat() * h;
	height_map[cols - 1]          = RandFloat() * h;
	height_map[(rows - 1) * cols] = RandFloat() * h;
	height_map[rows * cols - 1]   = RandFloat() * h;
}