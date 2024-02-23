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
		auto lines = read_lines("./day08/input/example1.txt");
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
		auto lines = read_lines("./day08/input/example1.txt");
		Input input{ .map = parse_lines(lines) };
		EXPECT_EQ(2, input.solve());
	}
	TEST(Solution, Example2) {
		auto lines = read_lines("./day08/input/example2.txt");
		Input input{ .map = parse_lines(lines) };
		EXPECT_EQ(6, input.solve());
	}
	TEST(Solution, Example3Part2) {
		auto lines = read_lines("./day08/input/example3.txt");
		Input input{ .map = parse_lines(lines) };
		EXPECT_EQ(6, input.solve_ghosts_naive());
		EXPECT_EQ(6, input.solve_ghosts());
	}
	TEST(Solution, Input) {
		auto lines = read_lines("./day08/input/input.txt");
		Input input{ .map = parse_lines(lines) };
		EXPECT_EQ(19783, input.solve());
	}
	TEST(Solution, InputPart2) {
		auto lines = read_lines("./day08/input/input.txt");
		Input input{ .map = parse_lines(lines) };
		// not 1998747435 (too low) -> int overflow
		// calculated manually: 9177460370549
		EXPECT_EQ(9177460370549, input.solve_ghosts());
	}
}
