#pragma once

#include <random>

#include "types.h"

template <typename TEngine = std::default_random_engine>
class random {
public:
	using distribution_t = std::uniform_int_distribution<int>;
	using param_t = distribution_t::param_type;

public:
	random() {
		std::random_device device;
		engine_.seed(device());
	}

	random(u32 seed) {
		engine_.seed(seed);
	}

	~random() {}

	void seed(u32 seed) {
		engine_.seed(seed);
	}

	int operator ()() {
		param_t p{ 0, INT_MAX };
		return distribution_(engine_, p);
	}

	int operator ()(int max) {
		param_t p{ 0, max };
		return distribution_(engine_, p);
	}

	int operator ()(int min, int max) {
		param_t p{ min, max };
		return distribution_(engine_, p);
	}

private:
	TEngine engine_;
	distribution_t distribution_;
};

template <typename TEngine = std::default_random_engine>
class random_real {
public:
	using distribution_t = std::uniform_real_distribution<float>;
	using param_t = distribution_t::param_type;

public:
	random_real() {
		std::random_device device;
		engine_.seed(device());
	}

	random_real(int seed) {
		engine_.seed(seed);
	}

	~random_real() {}

	void seed(int seed) {
		engine_.seed(seed);
	}

	float operator ()() {
		param_t p{ 0.0, FLT_MAX };
		return distribution_(engine_, p);
	}

	float operator ()(float max) {
		param_t p{ 0, max };
		return distribution_(engine_, p);
	}

	float operator ()(float min, float max) {
		param_t p{ min, max};
		return distribution_(engine_, p);
	}

private:
	TEngine engine_;
	distribution_t distribution_;
};

using random_t = random<>;
using random_real_t = random_real<>;

extern random_t g_random_int;
extern random_real_t g_random;