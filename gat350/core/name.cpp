#include "name.h"

#include "assert.h"
#include "string.h"

size_t Name::unique_id_ = 0;
char* Name::names_ = nullptr;

Name::Name(const char* string, bool unique) {
	ASSERT(strlen(string) < kMaxNameSize);

	char lower_string[kMaxNameSize];
	string_to_lower(string, lower_string, kMaxNameSize);

	if (unique) {
		std::string unique_string(lower_string);
		unique_string += std::to_string(unique_id_++);

		id_ = static_cast<u32>(std::hash<std::string>{}(unique_string.c_str()));
		index_ = id_ % kMaxEntries;
		strcpy_s(names_ + (index_ * kMaxNameSize), kMaxNameSize, string);
	} else {
		id_ = static_cast<u32>(std::hash<std::string>{}(lower_string));
		index_ = id_ % kMaxEntries;
		strcpy_s(names_ + (index_ * kMaxNameSize), kMaxNameSize, string);
	}
}

bool Name::operator ==(const Name& other) const {
	return id_ == other.id_;
}

bool Name::operator !=(const Name& other) const {
	return id_ != other.id_;
}

bool Name::operator<(const Name& other) const {
	return id_ < other.id_;
}

std::string Name::ToString() const {
	return std::string(names_ + (index_ * kMaxNameSize));
}

const char* Name::c_str() const {
	return names_ + (index_ * kMaxNameSize);
}

void Name::AllocNames() {
	names_ = new char[kMaxEntries * kMaxNameSize];
}

void Name::FreeNames() {
	delete names_;
}
