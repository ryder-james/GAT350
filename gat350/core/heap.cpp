#include "heap.h"

#include <new>
#include <iostream>

#include "types.h"
#include "assert.h"

#define MEMORY_SIG 0xDEADC0DE

heap::~heap() {
	report();
	//ASSERT_MSG(!allocations_, "memory leak");
}

void* heap::allocate(size_t size) {
	size_t total_size = size + sizeof(alloc_header);
	u8* memory = static_cast<u8*>(malloc(total_size));
	alloc_header* header = (alloc_header*)(memory);
	header->size_ = size;
	header->signature_ = MEMORY_SIG;

	header->next_ = alloc_head_;
	header->prev_ = nullptr;
	if (alloc_head_)
		alloc_head_->prev_ = header;

	alloc_head_ = header;

	allocated_ += size;
	allocated_total_ += size;
	if (allocated_ > allocated_peak_) allocated_peak_ = allocated_;

	allocations_++;
	allocations_total_++;

	void* ptr = memory + sizeof(alloc_header);

	return ptr;
}

void heap::free(void* ptr) {
	alloc_header* header = (alloc_header*)((u8*)ptr - sizeof(alloc_header));
	ASSERT(header->signature_ == MEMORY_SIG);

	free(header);
}

void heap::report() {
	std::cout << "current allocated: " << allocated_ << std::endl;
	std::cout << "total allocated: " << allocated_total_ << std::endl;
	std::cout << "peak allocated: " << allocated_peak_ << std::endl;
	std::cout << "current allocations: " << allocations_ << std::endl;
	std::cout << "total allocations: " << allocated_total_ << std::endl;
}

void heap::free(alloc_header* header) {
	if (!header->prev_) {
		alloc_head_ = header->next_;
	} else {
		header->prev_->next_ = header->next_;
	}

	if (header->next_) {
		header->next_->prev_ = header->prev_;
	}

	allocated_ -= header->size_;
	allocations_--;

	::free(header);
}
