#pragma once

#include <list>

template <typename T>
class PoolAllocator {
public:
	using ptr_type = BYTE *;

public:
	PoolAllocator(size_t size);
	~PoolAllocator();

	T* Get();
	void Free(void* address);

private:
	void* pool_ = nullptr;
	std::list<void*> freelist_;
};

template<typename T>
inline PoolAllocator<T>::PoolAllocator(size_t size) {
	pool_ = new char[size * sizeof(T)];
	for (size_t i = 0; i < size; i++) {
		ptr_type current = static_cast<ptr_type>(pool_) + (i * sizeof(T));
		freelist_.push_back(current);
	}
}

template<typename T>
inline PoolAllocator<T>::~PoolAllocator() {
	delete pool_;
}

template<typename T>
inline T* PoolAllocator<T>::Get() {
	T* element = nullptr;

	if (freelist_.empty() == false) {
		element = reinterpret_cast<T*>(freelist_.front());
		freelist_.pop_front();
	}

	return element;
}

template<typename T>
inline void PoolAllocator<T>::Free(void* element) {
	freelist_.push_front(element);
}
