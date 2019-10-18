#include "json.h"

#include "filesystem.h"

bool json::load(const char* filename, rapidjson::Document& document) {
	bool success = false;

	char* buffer;
	size_t size;

	if (filesystem::read_file(filename, (void**)& buffer, size)) {
		buffer[size - 1] = 0;
		document.Parse(buffer);
		success = document.IsObject();

		filesystem::free_file(buffer);
	}

	return success;
}

bool json::get(const rapidjson::Value& value, const char* property_name, int& _int) {
	auto iter = value.FindMember(property_name);

	if (iter == value.MemberEnd()) {
		return false;
	}

	auto& property = iter->value;
	if (!property.IsInt()) {
		return false;
	}

	_int = property.GetInt();

	return true;
}

bool json::get(const rapidjson::Value& value, const char* property_name, uint& _int) {
	auto iter = value.FindMember(property_name);

	if (iter == value.MemberEnd()) {
		return false;
	}

	auto& property = iter->value;
	if (!property.IsUint()) {
		return false;
	}

	_int = property.GetUint();

	return true;
}

bool json::get(const rapidjson::Value& value, const char* property_name, float& _float) {
	auto iter = value.FindMember(property_name);

	if (iter == value.MemberEnd()) {
		return false;
	}

	auto& property = iter->value;
	if (!property.IsFloat()) {
		return false;
	}

	_float = property.GetFloat();

	return true;
}

bool json::get(const rapidjson::Value& value, const char* property_name, std::string& _string) {
	auto iter = value.FindMember(property_name);

	if (iter == value.MemberEnd()) {
		return false;
	}

	auto& property = iter->value;
	if (!property.IsString()) {
		return false;
	}

	_string = property.GetString();

	return true;
}

bool json::get(const rapidjson::Value& value, const char* property_name, Name& _name) {
	auto iter = value.FindMember(property_name);

	if (iter == value.MemberEnd()) {
		return false;
	}

	auto& property = iter->value;
	if (!property.IsString()) {
		return false;
	}

	_name = property.GetString();

	return true;
}

bool json::get(const rapidjson::Value& value, const char* property_name, bool& _bool) {
	auto iter = value.FindMember(property_name);

	if (iter == value.MemberEnd()) {
		return false;
	}

	auto& property = iter->value;
	if (!property.IsBool()) {
		return false;
	}

	_bool = property.GetBool();

	return true;
}
