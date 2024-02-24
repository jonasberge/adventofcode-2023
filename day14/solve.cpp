// std
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
// local
#include "lib.h"

size_t solve(std::vector<std::string> const& lines)
{
	size_t result = 0;

	for (size_t j = 0; j < lines[0].size(); j++) {

		size_t current_score = lines.size();

		for (size_t i = 0; i < lines.size(); i++) {
			char c = lines[i][j];
			switch (c) {
			case '#':
				current_score = lines.size() - i - 1;
				break;
			case 'O':
				result += current_score;
				current_score -= 1;
				break;
			case '.':
				break;
			default:
				throw "invalid input character";
			}
		}
	}

	return result;
}

/*

my assumption is that for part 2 the cycles will at some point become stable
and we don't have to continue simulating, but we can reuse a range of previous simulation steps.

17:32-17:40
18:31-


*/

enum class Direction
{
	North,
	West,
	South,
	East
};

constexpr Direction opposite_direction(Direction direction)
{
	switch (direction) {
	case Direction::North: return Direction::South;
	case Direction::West: return Direction::East;
	case Direction::South: return Direction::North;
	case Direction::East: return Direction::West;
	}
}

constexpr Direction rotate_direction(Direction direction, bool clockwise)
{
	Direction rotated = direction;
	switch (direction) {
	case Direction::North: rotated = Direction::West; break;
	case Direction::West: rotated = Direction::South; break;
	case Direction::South: rotated = Direction::East; break;
	case Direction::East: rotated = Direction::North; break;
	}
	if (clockwise) {
		rotated = opposite_direction(rotated);
	}
	return rotated;
}

struct Platform
{
	std::vector<std::vector<char>> rocks;

	Platform(std::vector<std::string> lines)
	{
		rocks.reserve(lines.size());
		for (auto const& line : lines) {
			rocks.push_back(std::vector(line.begin(), line.end()));
		}
	}

	// calculates the score from part 1
	size_t score();

	// tilts the platform counter-clockwise (as described in the task).
	// from north to west to sout to east and back to north.
	void tilt(Direction direction);

	size_t hash()
	{
		std::size_t seed = rocks.size() * rocks[0].size();
		for (size_t i = 0; i < rocks.size(); i++) {
			for (size_t j = 0; j < rocks.size(); j++) {
				auto n = (size_t)rocks[i][j] * (i + 1) * (j + 7);
				seed ^= n + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
		}
		return seed;
	}

	void print()
	{
		for (auto const& line : rocks) {
			for (auto rock : line) {
				std::cout << rock;
			}
			std::cout << std::endl;
		}
		std::cout << std::flush;
	}
};

enum class Axis
{
	X,
	Y
};

size_t Platform::score()
{
	size_t result = 0;
	for (size_t j = 0; j < rocks[0].size(); j++) {
		for (size_t i = 0; i < rocks.size(); i++) {
			if (rocks[i][j] == 'O') {
				result += rocks.size() - i;
			}
		}
	}
	return result;
}

template<typename T, typename F>
void iterate_matrix(std::vector<std::vector<T>> matrix, Direction direction, F&& next) {
	switch (direction) {
	case Direction::North:
		for (int64_t k = 0; k < matrix[0].size(); k++) {
			bool first = true;
			for (int64_t i = matrix.size() - 1; i >= 0; i--) {
				next(first, matrix[i][k], i, k, Axis::Y, -1);
				first = false;
			}
		}
		break;
	case Direction::West:
		for (int64_t k = 0; k < matrix.size(); k++) {
			bool first = true;
			for (int64_t i = matrix[0].size() - 1; i >= 0; i--) {
				next(first, matrix[k][i], k, i, Axis::X, -1);
				first = false;
			}
		}
		break;
	case Direction::South:
		for (int64_t k = 0; k < matrix[0].size(); k++) {
			bool first = true;
			for (int64_t i = 0; i < matrix.size(); i++) {
				next(first, matrix[i][k], i, k, Axis::Y, +1);
				first = false;
			}
		}
		break;
	case Direction::East:
		for (int64_t k = 0; k < matrix.size(); k++) {
			bool first = true;
			for (int64_t i = 0; i < matrix[0].size(); i++) {
				next(first, matrix[k][i], k, i, Axis::X, +1);
				first = false;
			}
		}
		break;
	}
}

void Platform::tilt(Direction direction)
{
	int64_t current_index = 0;

	iterate_matrix(rocks, opposite_direction(direction),
		[&](bool first, char c, size_t y, size_t x, Axis axis, size_t step) {
			bool is_x = axis == Axis::X;
			size_t index = is_x ? x : y;
			if (first) {
				current_index = index;
			}
			switch (c) {
			case '#':
				current_index = index + step;
				break;
			case 'O':
				rocks[is_x ? y : current_index][is_x ? current_index : x] = c;
				if (current_index != index) {
					rocks[y][x] = '.';
				}
				current_index += step;
				break;
			case '.':
				break;
			default:
				throw "invalid input";
			}
		});
}

size_t solve2(std::vector<std::string> const& lines)
{
	static const size_t AMOUNT = 1000000000;

	Platform platform(lines);

	std::unordered_map<size_t, size_t> hash_index_map;

	auto direction = Direction::North;
	for (size_t i = 0; i < AMOUNT; i++) {
		platform.tilt(Direction::North);
		platform.tilt(Direction::West);
		platform.tilt(Direction::South);
		platform.tilt(Direction::East);
		auto hash = platform.hash();
		auto score = platform.score();
		if (hash_index_map.contains(hash)) {
			size_t begin = hash_index_map[hash];
			size_t end = i;
			size_t size = end - begin;
			i = AMOUNT - (AMOUNT - end) % size;
		}
		hash_index_map[hash] = i;
	}

	return platform.score();
}

// testing
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Tests
{
	using namespace testing;

	Platform north_tilted(std::vector<std::string> const& lines)
	{
		Platform platform(lines);
		platform.tilt(Direction::North);
		return platform;
	}

	TEST(Solve, Example1) {
		EXPECT_EQ(136, solve(read_lines("./day14/input/p1e1.txt")));
	}

	TEST(Solve, Input) {
		// attempt 1: 108840

		EXPECT_EQ(108840, solve(read_lines("./day14/input/in.txt")));
	}

	TEST(Platform, Score) {
		auto p1e1 = read_lines("./day14/input/p1e1.txt");
		auto in = read_lines("./day14/input/in.txt");
		EXPECT_EQ(solve(p1e1), north_tilted(p1e1).score());
		EXPECT_EQ(solve(in), north_tilted(in).score());
	}

	TEST(Solve, Part2Example1) {
		EXPECT_EQ(64, solve2(read_lines("./day14/input/p1e1.txt")));
	}

	TEST(Solve, Part2) {
		EXPECT_EQ(103445, solve2(read_lines("./day14/input/in.txt")));
	}
}
