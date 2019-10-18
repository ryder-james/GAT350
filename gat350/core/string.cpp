#include "string.h"

#include <sstream>

void string_to_lower(const char* source, char* destination, size_t size) {
	strcpy_s(destination, size, source);
	for (size_t i = 0; i < strlen(destination); i++) {
		destination[i] = std::tolower(destination[i]);
	}
}

void tokenize(const std::string& string, char delimiter, std::vector<std::string>& _tokens) {
	std::stringstream stream(string);

	std::string s;

	while (std::getline(stream, s, delimiter)) {
		_tokens.push_back(s);
	}
}

bool char_compare(char c1, char c2) {
	return c1 == c2 || std::toupper(c1) == std::toupper(c2);
}

bool string_compare(std::string& string1, std::string& string2, bool case_sensitive) {
	if (string1.size() != string2.size()) return false;
	if (case_sensitive) {
		return std::equal(string1.begin(), string1.end(), string2.begin(), string2.end());
	} else {
		return std::equal(string1.begin(), string1.end(), string2.begin(), string2.end(), &char_compare);
	}
}
