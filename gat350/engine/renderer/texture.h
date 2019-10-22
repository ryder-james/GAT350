#pragma once

#include "../engine.h"

class Texture {
public:
	Texture() {}
	~Texture();

	void CreateTexture(const std::string& filename, GLenum type = GL_TEXTURE_2D, GLuint unit = GL_TEXTURE0);
	void Bind();

	static u8* LoadImage(const std::string& filename, int& width, int& height, int& bpp);

protected:
	GLenum type_ = GL_TEXTURE_2D;
	GLuint unit_ = GL_TEXTURE0;
	GLuint texture_ = 0;
};