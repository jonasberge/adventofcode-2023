#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <numeric>
#include <cmath>

#include "solution1.h"

using namespace day8::solution1;

Map day8::solution1::parse_lines(std::vector<std::string> const& lines)
{
	Map map;
	std::istringstream iss(lines[0]);
	char c;
	while (iss >> c) {
		switch (c) {
		case 'L': map.steps.push_back(0); break;
		case 'R': map.steps.push_back(1); break;
		}
	}
	for (size_t i = 2; i < lines.size(); i++) {
		std::string const& line = lines[i];
		auto from = line.substr(0, 3);
		map.network[from] = { line.substr(7, 3), line.substr(12, 3) };
	}
	return map;
}

size_t day8::solution1::Input::solve()
{
	std::string current = "AAA";
	size_t step_index = 0;
	size_t step_count = 0;

	while (current != "ZZZ") {
		size_t step = map.steps[step_index];
		std::string const& next = map.network[current][step];

		current = next;
		step_count++;

		step_index++;
		if (step_index == map.steps.size()) {
			step_index = 0;
		}
	}

	return step_count;
}

size_t day8::solution1::Input::solve_ghosts_naive()
{
	std::vector<std::string> current;
	for (auto const& [key, _] : map.network) {
		if (key.ends_with('A')) {
			current.push_back(key);
		}
	}

	size_t step_index = 0;
	size_t step_count = 0;

	while (true) {
		size_t amount_ends_z = 0;

		size_t step = map.steps[step_index];
		for (size_t i = 0; i < current.size(); i++) {
			std::string const& next = map.network[current[i]][step];
			current[i] = next;
			if (next.ends_with('Z')) {
				amount_ends_z++;
			}
		}
		step_count++;

		step_index++;
		if (step_index == map.steps.size()) {
			step_index = 0;
		}

		// check if all end on z
		if (amount_ends_z == current.size()) {
			break;
		}
	}

	return step_count;
}

// source of gcd and lcm: https://stackoverflow.com/a/4229930
static long double gcd(long double a, long double b)
{
	for (;;)
	{
		if (a == 0) return b;
		b = fmodl(b, a);
		if (b == 0) return a;
		a = fmodl(a, b);
	}
}
static long double lcm(long double a, long double b)
{
	long double temp = gcd(a, b);
	return temp ? (a / temp * b) : 0;
}

size_t day8::solution1::Input::solve_ghosts()
{
	// should find the first time each of the starting points get to Z in the end.
	// then find the LCM (least common multiple) of those counts,
	// which is the amount at which each of them would "collide" and hit 'Z' at the same time.

	std::vector<std::string> current;
	for (auto const& [key, _] : map.network) {
		if (key.ends_with('A')) {
			current.push_back(key);
		}
	}

	size_t step_index = 0;
	size_t step_count = 0;

	constexpr auto MAX_VALUE = std::numeric_limits<size_t>::max();
	std::vector<size_t> step_lengths(current.size(), MAX_VALUE);

	while (true) {
		// increment beforehand, since we need the incremented number in the loop below.
		step_count++;

		size_t step = map.steps[step_index];
		for (size_t i = 0; i < current.size(); i++) {
			std::string const& next = map.network[current[i]][step];
			current[i] = next;
			if (next.ends_with('Z')) {
				step_lengths[i] = std::min(step_lengths[i], step_count);
			}
		}

		step_index++;
		if (step_index == map.steps.size()) {
			step_index = 0;
		}

		bool is_all_z = true;
		for (size_t step_length : step_lengths) {
			if (step_length == MAX_VALUE) {
				is_all_z = false;
				break;
			}
		}
		if (is_all_z) {
			break;
		}
	}

	return std::accumulate(step_lengths.begin(), step_lengths.end(), 1.0, lcm);
}
