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

// testing
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Tests
{
	using namespace testing;

	TEST(Solve, Example1) {
		ASSERT_EQ(136, solve(read_lines("./day14/input/p1e1.txt")));
	}

	TEST(Solve, Input) {
		// attempt 1: 108840

		ASSERT_EQ(108840, solve(read_lines("./day14/input/in.txt")));
	}
}
