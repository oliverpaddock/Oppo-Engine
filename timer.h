#pragma once

#include <type_traits>
#include <chrono>

template <typename T>
class Stopwatch final
{
public:

	using elapsed_resolution = T;

	Stopwatch()
	{
		Reset();
	}

	void Reset()
	{
		reset_time = clock.now();
	}

	elapsed_resolution Elapsed()
	{
		return std::chrono::duration_cast<elapsed_resolution>(clock.now() - reset_time);
	}

	long long ElapsedCount()
	{
		return Elapsed().count();
	}

private:

	std::chrono::high_resolution_clock clock;
	std::chrono::high_resolution_clock::time_point reset_time;
};