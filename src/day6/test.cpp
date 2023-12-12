#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "solution1.h"
#include "lib.h"

using namespace day6::solution1;
using namespace testing;

namespace day6
{
	TEST(ParseLines, ParseLine) {
		EXPECT_THAT(parse_line("Bla: 1 2 3 4 5"), UnorderedElementsAre(1, 2, 3, 4, 5));
	}
	TEST(ParseLines, ParseLines) {
		auto result = parse_lines({ "One: 1 2 3", "Two: 6 7 8" });
		EXPECT_THAT(result, UnorderedElementsAre(Race{ 1, 6 }, Race{ 2, 7 }, Race{ 3, 8 }));
	}

	TEST(Solution, Example1) {
		auto lines = read_lines("./src/day6/input/example1.txt");
		auto races = parse_lines(lines);
		Input input{ .races = races };
		EXPECT_EQ(288, input.solve());
	}
	TEST(Solution, Example2) {
		auto lines = read_lines("./src/day6/input/example2.txt");
		auto races = parse_lines(lines);
		Input input{ .races = races };
		EXPECT_EQ(71503, input.solve());
	}
	TEST(Solution, Input) {
		auto lines = read_lines("./src/day6/input/input.txt");
		auto races = parse_lines(lines);
		Input input{ .races = races };
		EXPECT_EQ(293046, input.solve());
	}
	TEST(Solution, Input2) {
		auto lines = read_lines("./src/day6/input/input2.txt");
		auto races = parse_lines(lines);
		Input input{ .races = races };
		EXPECT_EQ(35150181, input.solve());
	}
}
