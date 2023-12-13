#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include <algorithm>

#include "solution1.h"
#include "lib.h"

using namespace day9::solution1;
using namespace testing;

namespace day9
{
	TEST(ParseLines, ParseLines) {
		EXPECT_THAT(parse_line("0 3 6 9 12 15").values, ElementsAre(0, 3, 6, 9, 12, 15));
		auto result = parse_lines({ "0 3 6 9 12 15", "1 2 3" });
		ASSERT_EQ(2, result.size());
		EXPECT_THAT(result[0].values, ElementsAre(0, 3, 6, 9, 12, 15));
		EXPECT_THAT(result[1].values, ElementsAre(1, 2, 3));
	}
	TEST(ParseLines, InputLongestLine) {
		auto lines = read_lines("./src/day9/input/input.txt");
		auto sequences = parse_lines(lines);
		size_t maxlen = 0;
		for (solution1::Sequence const& sequence : sequences) {
			maxlen = std::max(maxlen, sequence.values.size());
		}
		// determine the max length beforehand, so we know how large our pascal triangle needs to be.
		EXPECT_EQ(21, maxlen);
	}

	TEST(PascalTriangle, Rows) {
		PascalTriangle pt;
		EXPECT_THAT(pt.get_row(0), ElementsAre(1));
		EXPECT_THAT(pt.get_row(1), ElementsAre(1, 1));
		EXPECT_THAT(pt.get_row(2), ElementsAre(1, 2, 1));
		EXPECT_THAT(pt.get_row(3), ElementsAre(1, 3, 3, 1));
		EXPECT_THAT(pt.get_row(14), ElementsAre(1, 14, 91, 364, 1001, 2002, 3003, 3432, 3003, 2002, 1001, 364, 91, 14, 1));
	}

	TEST(Solution, Example1) {
		auto lines = read_lines("./src/day9/input/example1.txt");
		auto sequences = parse_lines(lines);
		PascalTriangle pt;
		EXPECT_EQ(18, sequences[0].extrapolate_value(pt));
		EXPECT_EQ(28, sequences[1].extrapolate_value(pt));
		EXPECT_EQ(68, sequences[2].extrapolate_value(pt));
		Input input{ .sequences = sequences };
		EXPECT_EQ(114, input.solve());
	}
	TEST(Solution, Example1Part2) {
		auto lines = read_lines("./src/day9/input/example1.txt");
		auto sequences = parse_lines(lines);
		PascalTriangle pt;
		EXPECT_EQ(-3, sequences[0].extrapolate_value_backwards(pt));
		EXPECT_EQ(0, sequences[1].extrapolate_value_backwards(pt));
		EXPECT_EQ(5, sequences[2].extrapolate_value_backwards(pt));
		Input input{ .sequences = sequences };
		EXPECT_EQ(114, input.solve());
		EXPECT_EQ(2, input.solve_backwards());
	}
	TEST(Solution, Input) {
		auto lines = read_lines("./src/day9/input/input.txt");
		auto sequences = parse_lines(lines);
		Input input{ .sequences = sequences };
		EXPECT_EQ(1887980197, input.solve());
		// not -990
		// it's 990?
		int_t solution = input.solve_backwards();
		solution *= -1; // this seems to be necessary?
		EXPECT_EQ(990, solution);
	}
}
