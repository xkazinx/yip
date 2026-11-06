#pragma once
#include <random>

struct Random
{
	unique_ptr<std::mt19937> _mt;
	std::random_device _rd;

	Random();

	~Random();

	std::mt19937 & get() { return *_mt.get(); }

	template<typename T>
	T Generate(const T & from, const T & to) {
		if constexpr (std::is_floating_point_v<T>) {
			std::uniform_real_distribution<T> d(from, to);
			return d(get());
		}
		else {
			std::uniform_int_distribution<T> d(from, to);
			return d(get());
		}
	}

	bool Probability(const float num);
};
