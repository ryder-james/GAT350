#pragma once

#include <chrono>
#include <ratio>

template<typename TPeriod = std::chrono::high_resolution_clock::period>
class timer {
public:
	using clock = std::chrono::high_resolution_clock;
	using clock_duration = std::chrono::duration<clock::rep, TPeriod>;

public:
	timer() : start_point_(clock::now()) {}

	void reset() { start_point_ = clock::now(); }

	clock::rep elapsed_time() const {
		clock_duration duration = std::chrono::duration_cast<clock_duration>(clock::now() - start_point_);
		return duration.count();
	}

	double seconds() {
		clock_duration duration = std::chrono::duration_cast<clock_duration>(clock::now() - start_point_);
		return duration.count() / static_cast<double>(TPeriod::den);
	}

private:
	clock::time_point start_point_;
};

class frame_timer : public timer<> {
public:
	frame_timer() : 
		dt(0), 
		fps(0), 
		frame_count(0), 
		prev_frame_time(clock::now()), 
		start_frame_time(clock::now()) 
	{}

	void tick() {
		clock_duration duration = clock::now() - prev_frame_time;
		prev_frame_time = clock::now();
		dt = duration.count() / static_cast<float>(clock::period::den);
		frame_count++;
		if (frame_count == 100) {
			duration = clock::now() - start_frame_time;
			start_frame_time = clock::now();
			float elapsed = duration.count() / static_cast<float>(clock::period::den);
			fps = frame_count / elapsed;
			frame_count = 0;
		}
	}

public:
	float dt;
	float fps;

private:
	size_t frame_count;
	clock::time_point prev_frame_time;
	clock::time_point start_frame_time;
};

using ns_timer = timer<std::nano>;
using mis_timer = timer<std::micro>;
using ms_timer = timer<std::milli>;
using s_timer = timer<std::ratio<1, 1>>;

extern frame_timer g_timer;