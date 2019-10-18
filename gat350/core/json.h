#pragma once

#include <string>
#include <document.h>

#include "..\\core\name.h"

using uint = unsigned int;

namespace json {
	bool load(const char* filename, rapidjson::Document& document);
	bool get(const rapidjson::Value& value, const char* property_name, int& _int);
	bool get(const rapidjson::Value& value, const char* property_name, uint& _int);
	bool get(const rapidjson::Value& value, const char* property_name, float& _float);
	bool get(const rapidjson::Value& value, const char* property_name, std::string& _string);
	bool get(const rapidjson::Value& value, const char* property_name, Name& _name);
	bool get(const rapidjson::Value& value, const char* property_name, bool& _bool);
}