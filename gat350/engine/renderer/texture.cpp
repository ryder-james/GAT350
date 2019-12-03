#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Texture::~Texture() {

}

bool Texture::Create(const Name& name) {
	CreateTexture(name.c_str());

	return true;
}

void Texture::CreateTexture(const std::string& filename, GLenum type, GLuint unit) {
	type_ = type;
	unit_ = unit;

	int width, height, channels;

	u8* data = LoadImage(filename, width, height, channels);
	ASSERT(data);

	glGenTextures(1, &texture_);
	glBindTexture(type, texture_);

	GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
	glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#ifdef STB_IMAGE_IMPLEMENTATION
	stbi_image_free(data);
#else
	delete data;
#endif
}

void Texture::CreateTexture(u32 width, u32 height, GLenum format, GLenum type, GLuint unit) {
	type_ = type;
	unit_ = unit;

	glGenTextures(1, &texture_);
	glBindTexture(type, texture_);

	glTexImage2D(type, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::CreateCubeTexture(const std::vector<std::string>& filenames, GLuint unit) {
	type_ = GL_TEXTURE_CUBE_MAP;
	unit_ = unit;

	glGenTextures(1, &texture_);
	Bind();

	int width, height, channels;

	for (size_t i = 0; i < filenames.size(); i++) {
		u8* data = LoadImage(filenames[i], width, height, channels);
		ASSERT(data);

		GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		
#ifdef STB_IMAGE_IMPLEMENTATION
		stbi_image_free(data);
#else
		delete data;
#endif
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void Texture::Bind() {
	glActiveTexture(unit_);
	glBindTexture(type_, texture_);
}

std::vector<std::string> Texture::GenerateCubeMapNames(const std::string& basename, const std::vector<std::string>& suffixes, const std::string& extension) {
	std::vector<std::string> names;
	for (size_t i = 0; i < 6; i++)
	{
		std::string name = basename + suffixes[i] + extension;
		names.push_back(name);
	}

	return names;
}

#ifdef STB_IMAGE_IMPLEMENTATION
u8* Texture::LoadImage(const std::string& filename, int& width, int& height, int& channels) {
	stbi_set_flip_vertically_on_load(false);
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
