#pragma once

#include "core.h"

void string_to_lower(const char* source, char* destination, size_t size);
void tokenize(const std::string& string, char delimiter, std::vector<std::string>& _tokens);
bool char_compare(char c1, char c2);
bool string_compare(std::string& string1, std::string& string2, bool case_sensitive = false);