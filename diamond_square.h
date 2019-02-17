#pragma once

#include <string>
#include <vector>
#include <random>

#include <FreeImage.h>

#include "texture.h"

using std::string;
using std::vector;

bool SaveImage(const string &filename, const std::vector<GLfloat> &data, size_t w, size_t h);

void PowHeightMap(float p, vector<GLfloat> &hm, size_t cols, size_t rows);
GLuint DiamondSquare(std::vector<GLfloat> &position_vec, size_t cols, size_t rows, float size);
void Init(vector<GLfloat> &height_map, size_t cols, size_t rows, float h);

inline float RandFloat() {
	return rand() / (RAND_MAX + 1.0f);
}

inline float RandNegFloat() {
	return rand() / (RAND_MAX + 1.0f) * 2.0f - 1.0f;
}