#pragma once

class ref_count {
public:
	ref_count() {}

	void add_ref() { count_++; }
	void release() { count_--; }

	size_t count() { return count_; }

	ref_count(const ref_count&) = delete;
	ref_count& operator =(const ref_count&) = delete;

private:
	size_t count_ = 0;
};