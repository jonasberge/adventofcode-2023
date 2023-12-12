#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "solution1.h"
#include "lib.h"

using namespace day8::solution1;
using namespace testing;

namespace day8
{
	using ArrayPair = std::array<std::string, 2>;

	TEST(ParseLines, Example1) {
		auto lines = read_lines("./src/day8/input/example1.txt");
		// only handle the first 3 lines
		std::vector<std::string> trimmed_lines(lines.begin(), lines.begin() + 5);
		auto map = parse_lines(trimmed_lines);
		EXPECT_THAT(map.steps, ElementsAre(1, 0));
		EXPECT_THAT(map.network, UnorderedElementsAre(
			Pair("AAA", ArrayPair{ "BBB", "CCC" }),
			Pair("BBB", ArrayPair{ "DDD", "EEE" }),
			Pair("CCC", ArrayPair{ "ZZZ", "GGG" })
		));
	}
	TEST(Solution, Example1) {
		auto lines = read_lines("./src/day8/input/example1.txt");
		Input input{ .map = parse_lines(lines) };
		EXPECT_EQ(2, input.solve());
	}
	TEST(Solution, Example2) {
		auto lines = read_lines("./src/day8/input/example2.txt");
		Input input{ .map = parse_lines(lines) };
		EXPECT_EQ(6, input.solve());
	}
	TEST(Solution, Input) {
		auto lines = read_lines("./src/day8/input/input.txt");
		Input input{ .map = parse_lines(lines) };
		EXPECT_EQ(19783, input.solve());
	}
}
