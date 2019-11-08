#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::~Texture() {

}

void Texture::CreateTexture(const std::string& filename, GLenum type, GLuint unit) {
	type_ = type;
	unit_ = unit;

	int width, height, channels;

	u8* data = LoadImage(filename, width, height, channels);
	ASSERT(data);

	glGenTextures(1, &texture_);
	Bind();

	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	stbi_image_free(data);
}

void Texture::Bind() {
	glActiveTexture(unit_);
	glBindTexture(type_, texture_);
}

#ifdef STB_IMAGE_IMPLEMENTATION
u8* Texture::LoadImage(const std::string& filename, int& width, int& height, int& channels) {
	stbi_set_flip_vertically_on_load(true);
	u8* image = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	return image;
}
#else
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
#endif
