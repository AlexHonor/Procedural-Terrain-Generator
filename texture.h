#pragma once


#include "common.h"

#include <glad/glad.h>
#include <Freeimage.h>

#include <string>

using std::string;

GLuint LoadTexture(const string &name);
void BindToSlot(GLuint tex, GLuint slot);
GLuint TextureFromPixels(GLubyte *pixels, size_t w, size_t h);
GLuint TextureFromPixelsNoMipMap(GLubyte *pixels, size_t w, size_t h);