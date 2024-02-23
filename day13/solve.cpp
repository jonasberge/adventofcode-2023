// std
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <functional>
#include <iostream>
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

/*

- reflections are only between lines, not on lines
- reflections are either on rows or columns
- rows/columns that go beyond the reflection are ignored

*/

struct Pattern
{
	std::vector<std::string> lines;
};

std::vector<Pattern> parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Pattern> result;
	Pattern current_pattern;
	for (auto const& line : lines) {
		if (line.size() == 0) {
			if (current_pattern.lines.size() > 0) {
				result.push_back(current_pattern);
			}
			current_pattern = {};
			continue;
		}
		current_pattern.lines.push_back(line);
	}
	if (current_pattern.lines.size() > 0) {
		result.push_back(current_pattern);
	}
	return result;
}

size_t solve(const Pattern& input)
{
	std::vector<uint64_t> prev_bitmasks;
	size_t backtrack_index = 0;
	size_t mirror_index = 0;

	for (size_t i = 0; i < input.lines.size(); i++) {
		int64_t bitmask = 0;
		for (size_t j = 0; j < input.lines[i].size(); j++) {
			bitmask <<= 1;
			bitmask |= input.lines[i][j] == '#' ? 1 : 0;
		}
		if (i > 0 && mirror_index == 0 && bitmask == prev_bitmasks.back()) {
			mirror_index = i;
			backtrack_index = prev_bitmasks.size() - 1;
			prev_bitmasks.push_back(bitmask);
		}
		if (mirror_index > 0) {
			if (bitmask == prev_bitmasks[backtrack_index]) {
				if (backtrack_index == 0 || i == input.lines.size() - 1) {
					return 100 * mirror_index; // rows above * 100
				}
				backtrack_index--;
			}
			else {
				// go back to where we left off
				i = mirror_index;
				mirror_index = 0;
				backtrack_index = 0;
			}
		}
		else {
			prev_bitmasks.push_back(bitmask);
		}
	}

	prev_bitmasks.clear();
	backtrack_index = 0;
	mirror_index = 0;

	// fixme: code duplication
	for (size_t j = 0; j < input.lines[0].size(); j++) {
		uint64_t bitmask = 0;
		for (size_t i = 0; i < input.lines.size(); i++) {
			bitmask <<= 1;
			bitmask |= input.lines[i][j] == '#' ? 1 : 0;
		}
		if (j > 0 && mirror_index == 0 && bitmask == prev_bitmasks.back()) {
			mirror_index = j;
			backtrack_index = prev_bitmasks.size() - 1;
			prev_bitmasks.push_back(bitmask);
		}
		if (mirror_index > 0) {
			if (bitmask == prev_bitmasks[backtrack_index]) {
				if (backtrack_index == 0 || j == input.lines[0].size() - 1) {
					return mirror_index; // cols before
				}
				backtrack_index--;
			}
			else {
				// go back to where we left off
				j = mirror_index;
				mirror_index = 0;
				backtrack_index = 0;
			}
		}
		else {
			prev_bitmasks.push_back(bitmask);
		}
	}

	_ASSERT(false); // should be unreachable
	return 0;
}

size_t solve_all(std::vector<Pattern> patterns)
{
	size_t sum = 0;
	// size_t i = 0;
	for (auto const& pattern : patterns) {
		// std::cout << (i++) << std::endl << std::flush;
		sum += solve(pattern);
	}
	return sum;
}

// testing
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Tests
{
	using namespace testing;

	TEST(ParseLines, Parses) {
		auto patterns = parse_lines(read_lines("day13/input/p1e1.txt"));
		ASSERT_EQ(2, patterns.size());
		EXPECT_EQ(7, patterns[0].lines.size());
		EXPECT_EQ(7, patterns[1].lines.size());
	}

	TEST(Solve, Example1) {
		auto patterns = parse_lines(read_lines("day13/input/p1e1.txt"));
		ASSERT_EQ(2, patterns.size());
		EXPECT_EQ(5, solve(patterns[0]));
		EXPECT_EQ(400, solve(patterns[1]));
		EXPECT_EQ(405, solve_all(patterns));
	}
	
	TEST(Solve, Part1) {
		// attempt 1: 35210

		auto patterns = parse_lines(read_lines("day13/input/in.txt"));
		EXPECT_EQ(35210, solve_all(patterns));
	}
}
