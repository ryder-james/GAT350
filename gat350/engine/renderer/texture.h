#pragma once

#include "../engine.h"

class Texture {
public:
	static u8* LoadImage(const std::string& filename, int& width, int& height, int& bpp);
};