#include "filesystem.h"

#include <experimental/filesystem>
#include <fstream>

namespace fs = std::experimental::filesystem;

bool filesystem::file_exists(const char* filename) {
	return fs::exists(filename) && fs::is_regular_file(filename);
}

bool filesystem::file_size(const char* filename, size_t& size) {
	bool success = false;

	if (file_exists(filename)) {
		success = true;
		size = static_cast<size_t>(fs::file_size(filename));
	}

	return success;
}

bool filesystem::read_file(const char* filename, std::string& string) {
	bool success = false;
	if (file_exists(filename)) {
		std::ifstream stream(filename, std::ios::binary);
		if (stream.is_open()) {
			size_t size;
			if (file_size(filename, size)) {
				success = true;
				string.resize(size + 1, '\0');

				stream.read(&string[0], size);
				stream.close();
			}
		}
	}

	return success;
}

bool filesystem::read_file(const char* filename, void** buffer, size_t& size) {
	bool success = false;

	if (file_size(filename, size)) {
		std::ifstream stream(filename, std::ios::binary);
		if (stream.is_open()) {
			success = true;

			*buffer = new char[size];
			stream.read(static_cast<char*>(*buffer), size);
			stream.close();
		}
	}

	return success;
}

bool filesystem::write_file(const char* filename, void* buffer, size_t size) {
	bool success = false;

	std::ofstream stream(filename, std::ios::binary);
	if (stream.is_open()) {
		success = true;
		stream.write(static_cast<char*>(buffer), size);
		stream.close();
	}

	return success;
}

void filesystem::free_file(void* buffer) {
	delete buffer;
}

bool filesystem::delete_file(const char* filename) {
	bool success = false;

	if (file_exists(filename)) {
		success = fs::remove(filename);
	}

	return success;
}

bool filesystem::copy_file(const char* source, const char* destination, bool overwrite) {
	bool success = false;

	if (file_exists(source)) {
		success = fs::copy_file(source, destination, (overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::skip_existing));
	}

	return success;
}

bool filesystem::directory_exists(const char* directory) {
	return fs::exists(directory) && fs::is_directory(directory);
}

bool filesystem::copy_directory(const char* source, const char* destination, bool recursive) {
	bool success = false;

	if (directory_exists(source)) {
		success = true;
		fs::copy(source, destination, (recursive ? fs::copy_options::recursive : fs::copy_options::none));
	}

	return success;
}

bool filesystem::create_directory(const char* directory) {
	return fs::create_directories(directory);
}

bool filesystem::delete_directory(const char* directory) {
	bool success = false;

	if (directory_exists(directory)) {
		success = true;
		fs::remove_all(directory);
	}

	return success;
}

bool filesystem::delete_empty_directory(const char* directory) {
	bool success = false;

	if (directory_exists(directory) && fs::is_empty(directory)) {
		success = true;
		fs::remove(directory);
	}

	return success;
}

void filesystem::set_current_path(const char* pathname) {
	fs::current_path(pathname);
}

void filesystem::get_current_path(char* pathname, size_t size) {
	strcpy_s(pathname, size, fs::current_path().string().c_str());
}
