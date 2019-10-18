#include "texture.h"

u8* Texture::LoadImage(const std::string& filename, int& width, int& height, int& bpp) {
	u8* image = nullptr;
	
	std::ifstream stream(filename, std::ios::binary);
	if (stream.is_open()) {
		char header[54];
		size_t data_offset = 0;
		size_t image_size = 0;

		stream.read(header, 54);

		width = *(int*) &(header[0x12]);
		height = *(int*) &(header[0x16]);
		bpp = *(int*) &(header[0x1C]);
		image_size = *(int*) &(header[0x22]);
		data_offset = *(int*) &(header[0x0A]);

		image = new u8[image_size];
		stream.seekg(data_offset);
		stream.read((char*)image, image_size);

		stream.close();
	}

	return image;
}
