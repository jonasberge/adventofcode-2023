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



*/

namespace helper {
	
}

struct Input
{

};

Input parse_line(std::string const& line)
{
	std::istringstream iss(line);
	Input input;



	return input;
}

std::vector<Input> parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Record> result;
	result.reserve(lines.size());
	for (auto const& line : lines) {
		result.push_back(parse_line(line));
	}
	return result;
}

size_t solve(const Input& input)
{


	return 0;
}

// testing
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Tests
{
	using namespace testing;

	TEST(ParseLine, Parses) {
		auto r = parse_line("?###???????? 3,2,1");
		EXPECT_EQ(r.pattern, "?###????????");
		EXPECT_THAT(r.sequence, ElementsAre(3, 2, 1));
	}

	
}
