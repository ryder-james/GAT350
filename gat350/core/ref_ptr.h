#pragma once

#include "ref_count.h"

template<typename T>
class ref_ptr {
public:
	ref_ptr(T* ptr) : ptr_(ptr) {
		if (ptr) {
			ref_count_ = new ref_count();
			ref_count_->add_ref();
		}
	}

	ref_ptr(const ref_ptr<T>& rp) {
		operator =(rp);
	}

	~ref_ptr() { 
		if (ptr_) { 
			ref_count_->release();
			if (!ref_count_->count()) {
				delete ref_count_;
				delete ptr_;
			}
		}
	}

	ref_ptr& operator =(const ref_ptr& rp) {
		if (ptr_ != rp.ptr_) {
			if (ptr_) {
				ref_count_->release();
				if (!ref_count_->count()) {
					delete ref_count_;
					delete ptr_;
				}
			}

			ptr_ = rp.ptr_;
			ref_count_ = rp.ref_count_;
			ref_count_->add_ref();
		}

		return *this;
	}

	T* get() { return ptr_; }

	T& operator *() { return *ptr_; }
	T* operator ->() { return &ptr_; }

	// deleting functions

private:
	ref_count* ref_count_ = nullptr;
	T* ptr_ = nullptr;
};