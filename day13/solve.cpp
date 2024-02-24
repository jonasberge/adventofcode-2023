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

/*

we firstly need to find the current reflection and store it, so we can exclude it later.
we also need to store, for each possible reflection row/column,
how many smudges there are, or how many different symbols/characters.
if we use the same bitmask strategy we can count the number of bits
when XORing the bitmasks of two rows/columns and sum the number of differences.
if that ever exceeds 1, then there is more than one smudge and we can discard that option.
we then find the (hopefully) single option that has exactly one "smudge".
to get the smudge location, we need to store the index of the bit that was set after XORing.
since there actually may only be 1 bit in the XOR result, instead of counting 1 bits,
we can just check if the XOR result is a power of 2.
the log2() of this result is the index/row/column of the smudge.

*/

struct ReflectionLine
{
	bool is_horizontal = true; // horizontal: row reflection, vertical: col reflection
	size_t index; // index of the row or col, depending on is_horizontal
	std::optional<std::pair<size_t, size_t>> smudge;

	size_t get_score() const
	{
		return is_horizontal ? index * 100 : index;
	}
};

void find_reflections(Pattern const& input, bool is_horizontal, std::vector<ReflectionLine>& out)
{
	std::vector<uint64_t> prev_bitmasks;
	size_t backtrack_index = 0;
	size_t mirror_index = 0;
	std::optional<std::pair<size_t, size_t>> smudge_position = std::nullopt;

	size_t outer_size = is_horizontal ? input.lines.size() : input.lines[0].size();
	for (size_t i = 0; i < outer_size; i++) {
		int64_t bitmask = 0;
		size_t inner_size = is_horizontal ? input.lines[i].size() : input.lines.size();
		for (size_t j = 0; j < inner_size; j++) {
			bitmask <<= 1;
			char value = is_horizontal ? input.lines[i][j] : input.lines[j][i];
			bitmask |= value == '#' ? 1 : 0;
		}
		if (i > 0 && mirror_index == 0) {
			mirror_index = i;
			backtrack_index = prev_bitmasks.size() - 1;
			prev_bitmasks.push_back(bitmask);
		}
		if (mirror_index == 0) {
			prev_bitmasks.push_back(bitmask);
			continue;
		}
		bool is_valid = bitmask == prev_bitmasks[backtrack_index];
		if (!is_valid && !smudge_position.has_value()) {
			auto smudges = bitmask ^ prev_bitmasks[backtrack_index];
			// if the xor result is a power of 2, it's a single bit and thus a single smudge.
			bool is_single_smudge = (smudges & (smudges - 1)) == 0;
			if (is_single_smudge) {
				is_valid = true;
				size_t a = i;
				if ((smudges & prev_bitmasks[backtrack_index]) > 0) {
					// the smudge is in the backtrack bitmask
					a = (mirror_index - 1) - (i - mirror_index);
				}
				size_t b = outer_size - (size_t)std::log2(smudges) - 1;
				smudge_position = is_horizontal ? std::make_pair(a, b) : std::make_pair(b, a);
			}
		}
		bool is_done = false;
		if (is_valid) {
			if (backtrack_index == 0 || i == outer_size - 1) {
				out.push_back(ReflectionLine{
					.is_horizontal = is_horizontal,
					.index = mirror_index,
					.smudge = smudge_position
				});
				is_done = true;
			}
			else {
				backtrack_index--;
			}
		}
		if (!is_valid || is_done) {
			// go back to where we left off
			i = mirror_index;
			mirror_index = 0;
			backtrack_index = 0;
			smudge_position = std::nullopt;
		}
	}
}

size_t solve2(Pattern const& input)
{
	std::vector<ReflectionLine> reflections;
	find_reflections(input, true, reflections);
	find_reflections(input, false, reflections);

	// must be one original reflection
	// and one reflection with exactly one smudge.
	_ASSERT(reflections.size() == 2);

	auto const& it = std::find_if(reflections.begin(), reflections.end(), [](auto const& e) {
		return e.smudge.has_value();
	});
	_ASSERT(it != reflections.end());

	return it->get_score();
}

size_t solve2_all(std::vector<Pattern> patterns)
{
	size_t sum = 0;
	for (auto const& pattern : patterns) {
		sum += solve2(pattern);
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

	TEST(Solve2, Example1) {
		auto patterns = parse_lines(read_lines("day13/input/p1e1.txt"));
		EXPECT_EQ(300, solve2(patterns[0]));
		EXPECT_EQ(100, solve2(patterns[1]));
		EXPECT_EQ(400, solve2_all(patterns));
	}

	TEST(Solve2, Part2) {
		// attempt 1: 31974

		auto patterns = parse_lines(read_lines("day13/input/in.txt"));
		EXPECT_EQ(31974, solve2_all(patterns));
	}
}
