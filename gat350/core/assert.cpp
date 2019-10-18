#include "assert.h"

#include <iostream>

void _assert(bool expression, const char* expression_str, const char* file_name, int line, const char* func_name, const char* message) {
	if (!expression) {
		std::cout << "assertion failed: " << expression_str << std::endl;

		if (message) {
			std::cout << message << std::endl;
		}

		std::cout << "source: " << file_name << std::endl;
		std::cout << "line: " << line << std::endl;
		std::cout << "function: " << func_name << std::endl;
		abort();
	}
}
