#include "texture.h"

GLuint TextureFromPixels(GLubyte *pixels, size_t w, size_t h) {
	GLuint texturaID; //id на gpu	
	glGenTextures(1, &texturaID); GL_CHECK_ERRORS; // создание текстуры на gpu
	glBindTexture(GL_TEXTURE_2D, texturaID); GL_CHECK_ERRORS; //сделать текстуру текущей
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)pixels); GL_CHECK_ERRORS; //отправить текстуру на gpu

	glGenerateMipmap(GL_TEXTURE_2D);

	//способо сэмплирования 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GL_CHECK_ERRORS; //билинейная
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); GL_CHECK_ERRORS;//трилинейная с мипмапингом

	//способ интерпретации координат текстуры по разным координатам
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); GL_CHECK_ERRORS;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); GL_CHECK_ERRORS;

	glBindTexture(GL_TEXTURE_2D, 0);//текущая текстура 
	
	return texturaID;
}

GLuint TextureFromPixelsNoMipMap(GLubyte *pixels, size_t w, size_t h) {
	GLuint texturaID;

	glGenTextures(1, &texturaID); GL_CHECK_ERRORS;
	glBindTexture(GL_TEXTURE_2D, texturaID); GL_CHECK_ERRORS;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)pixels); GL_CHECK_ERRORS;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); GL_CHECK_ERRORS;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); GL_CHECK_ERRORS;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); GL_CHECK_ERRORS;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); GL_CHECK_ERRORS;

	glBindTexture(GL_TEXTURE_2D, 0);

	return texturaID;
}

GLuint LoadTexture(const string &name) {
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(name.c_str(), 0);
	FIBITMAP* imagen = FreeImage_Load(formato, name.c_str());

	FIBITMAP* temp = imagen;
	imagen = FreeImage_ConvertTo32Bits(imagen);
	FreeImage_Unload(temp);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	GLubyte* textura = new GLubyte[4 * w*h];
	char* pixeles = (char*)FreeImage_GetBits(imagen);

	for (int j = 0; j<w*h; j++) {
		textura[j * 4 + 0] = pixeles[j * 4 + 2];
		textura[j * 4 + 1] = pixeles[j * 4 + 1];
		textura[j * 4 + 2] = pixeles[j * 4 + 0];
		textura[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	return TextureFromPixels(textura, w, h);
}

void BindToSlot(GLuint tex, GLuint slot) {
	glActiveTexture(GL_TEXTURE0 + slot); GL_CHECK_ERRORS;
	glBindTexture(GL_TEXTURE_2D, tex); GL_CHECK_ERRORS;
	glActiveTexture(GL_TEXTURE0); GL_CHECK_ERRORS;
}