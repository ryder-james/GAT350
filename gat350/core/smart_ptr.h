#pragma once

template<typename T>
class smart_ptr {
public:
	smart_ptr(T* ptr) : ptr_(ptr) {}
	~smart_ptr() { if (ptr_) delete ptr_; }

	T* get() { return ptr_; }

	T& operator *() { return *ptr_; }
	T* operator ->() { return &ptr_; }

	// deleting functions
	smart_ptr(const smart_ptr<T>&) = delete;
	smart_ptr& operator =(const smart_ptr&) = delete;

private:
	T* ptr_ = nullptr;
};