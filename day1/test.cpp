#include <gtest/gtest.h>

#include "solution1.h"
#include "lib.h"

namespace day1
{
	class Tests : public testing::Test
	{
	protected:
		day1::Solution1 s;
	};

	// part 1
	TEST_F(Tests, HandlesNoDigits) {
		EXPECT_EQ(0, s.handle_line(""));
		EXPECT_EQ(0, s.handle_line("aaaaa"));
		EXPECT_EQ(0, s.handle_line("kjhjsheg"));
	}
	TEST_F(Tests, HandlesSingleDigit) {
		EXPECT_EQ(22, s.handle_line("2"));
		EXPECT_EQ(99, s.handle_line("9c"));
		EXPECT_EQ(11, s.handle_line("aa1bb"));
	}
	TEST_F(Tests, HandlesFirstDigitZero) {
		EXPECT_EQ(3, s.handle_line("03"));
	}
	TEST_F(Tests, HandlesLastDigitZero) {
		EXPECT_EQ(30, s.handle_line("30"));
	}
	TEST_F(Tests, HandlesAllZeros) {
		EXPECT_EQ(0, s.handle_line("000"));
		EXPECT_EQ(0, s.handle_line("0"));
	}
	TEST_F(Tests, HandlesMultipleDigits) {
		EXPECT_EQ(12, s.handle_line("1aaa2"));
		EXPECT_EQ(13, s.handle_line("13"));
		EXPECT_EQ(19, s.handle_line("zzz1X3x3x3x9"));
	}
	TEST_F(Tests, HandlesMultipleLines) {
		EXPECT_EQ(11 + 12 + 33 + 10 + 9 + 0, s.handle_lines({ "1", "1aa2", "aaa3cc", "10", "09", "0000000" }));
	}
	TEST_F(Tests, SolutionPart1) {
		EXPECT_EQ(55002, s.handle_lines(read_lines("./day1/input/input.txt")));
	}
	TEST_F(Tests, SolutionPart1Example1) {
		EXPECT_EQ(142, s.handle_lines(read_lines("./day1/input/example1.txt")));
	}

	// part 2
	TEST_F(Tests, TransformsWrittenNumbers) {
		EXPECT_EQ("PRE2POST", s.transform_written_numbers("PREtwoPOST"));
		EXPECT_EQ("94", s.transform_written_numbers("ninefour"));
		EXPECT_EQ("a1b2c3123", s.transform_written_numbers("aonebtwocthree123"));
		EXPECT_EQ("123", s.transform_written_numbers("1two3"));
		EXPECT_EQ("123", s.transform_written_numbers("one2three"));
		EXPECT_EQ("123", s.transform_written_numbers("onetwothree"));
	}
	TEST_F(Tests, TransformsWithoutWrittenNumbers) {
		EXPECT_EQ("", s.transform_written_numbers(""));
		EXPECT_EQ("abc", s.transform_written_numbers("abc"));
		EXPECT_EQ("1", s.transform_written_numbers("1"));
	}
	TEST_F(Tests, TransformsOverlappingNumbersEach) {
		EXPECT_EQ("18", s.transform_written_numbers("oneight"));
		EXPECT_EQ("79", s.transform_written_numbers("sevenine"));
	}
	TEST_F(Tests, SolutionPart2Example2) {
		EXPECT_EQ(281, s.handle_lines(s.transformed_written_numbers(read_lines("./day1/input/example2.txt"))));
	}
	TEST_F(Tests, SolutionPart2) {
		EXPECT_EQ(55093, s.handle_lines(s.transformed_written_numbers(read_lines("./day1/input/input.txt"))));
	}
}
