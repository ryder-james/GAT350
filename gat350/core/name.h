#pragma once

#include "types.h"

#include <string>

class Name {
public:
	Name() : id_(0), index_(0) {}
	Name(const char* string, bool unique = false);

	bool operator == (const Name& other) const;
	bool operator != (const Name& other) const;
	bool operator < (const Name& other) const;

	std::string ToString() const;
	const char* c_str() const;

	size_t GetID() const { return id_; }

	static void AllocNames();
	static void FreeNames();

private:
	size_t id_;
	size_t index_;

	static const u32 kMaxEntries = 65536;
	static const u32 kMaxNameSize = 64;

	static size_t unique_id_;
	static char* names_;
};

