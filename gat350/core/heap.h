#pragma once

class heap {
public:
	struct alloc_header {
		size_t signature_ = 0;
		size_t size_ = 0;
		alloc_header* next_ = nullptr;
		alloc_header* prev_ = nullptr;
	};

public:
	heap() {}
	~heap();

	void* allocate(size_t size);
	void free(void* ptr);

	void report();

private:
	void free(alloc_header* header);

private:
	size_t allocated_ = 0;
	size_t allocated_total_ = 0;
	size_t allocated_peak_ = 0;
	size_t allocations_ = 0;
	size_t allocations_total_ = 0;

	alloc_header* alloc_head_ = nullptr;
};