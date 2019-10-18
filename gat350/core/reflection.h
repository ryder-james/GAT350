#pragma once

#include "types.h" // -NEW-

//-REMOVE-

#define REFLECT_FORM(data) {#data, data}

template <typename T>
struct property_t {
	const char* name;
	T data;
};

template <typename T>
bit_mask_t CreateBitMask(T* properties, size_t size, std::vector<std::string>& _tokens) {
	bit_mask_t bit_mask;

	for (std::string& token : _tokens) {
		for (size_t i = 0; i < size; i++) {
			std::string s(properties[i].name);
			if (string_compare(s, token, false)) {
				bit_mask.set(properties[i].data);
			}
		}
	}

	return bit_mask;
}