#include <gtest/gtest.h>

#include "day1_solution1.h"
#include "lib.h"

namespace day1 {
	// should be handleded as zero,
	// since the sum shouldn't be affected by a line without digits.
	TEST(solution1, handles_no_digit_as_zero) {
		EXPECT_EQ(day1_solution1::handle_line(""), 0);
		EXPECT_EQ(day1_solution1::handle_line("aaaaa"), 0);
		EXPECT_EQ(day1_solution1::handle_line("kjhjsheg"), 0);
	}
	TEST(solution1, handles_single_digit) {
		EXPECT_EQ(day1_solution1::handle_line("2"), 22);
		EXPECT_EQ(day1_solution1::handle_line("9c"), 99);
		EXPECT_EQ(day1_solution1::handle_line("aa1bb"), 11);
	}
	TEST(solution1, handles_zero_start_digit) {
		EXPECT_EQ(day1_solution1::handle_line("03"), 3);
	}
	TEST(solution1, handles_zero_end_digit) {
		EXPECT_EQ(day1_solution1::handle_line("30"), 30);
	}
	TEST(solution1, handles_all_zeros) {
		EXPECT_EQ(day1_solution1::handle_line("000"), 0);
		EXPECT_EQ(day1_solution1::handle_line("0"), 0);
	}
	TEST(solution1, handles_one_or_more_digits) {
		EXPECT_EQ(day1_solution1::handle_line("1aaa2"), 12);
		EXPECT_EQ(day1_solution1::handle_line("13"), 13);
		EXPECT_EQ(day1_solution1::handle_line("zzz1X3x3x3x9"), 19);
	}
	TEST(solution1, handles_multiple_lines) {
		EXPECT_EQ(day1_solution1::handle_lines({
				"1", "1aa2", "aaa3cc", "10", "09", "0000000"
			}), 11 + 12 + 33 + 10 + 9 + 0);
	}
	TEST(solution1, solution_part1) {
		EXPECT_EQ(55002, day1_solution1::handle_lines(read_lines("./input/day1.txt")));
	}
	TEST(solution1, solution_part1_example1) {
		EXPECT_EQ(142, day1_solution1::handle_lines(read_lines("./input/day1-example1.txt")));
	}

	TEST(solution1, transforms_written_numbers) {
		EXPECT_EQ("PRE2POST", day1_solution1::transform_written_numbers("PREtwoPOST"));
		EXPECT_EQ("94", day1_solution1::transform_written_numbers("ninefour"));
		EXPECT_EQ("a1b2c3123", day1_solution1::transform_written_numbers("aonebtwocthree123"));
		EXPECT_EQ("123", day1_solution1::transform_written_numbers("1two3"));
		EXPECT_EQ("123", day1_solution1::transform_written_numbers("one2three"));
		EXPECT_EQ("123", day1_solution1::transform_written_numbers("onetwothree"));
	}
	TEST(solution1, transforms_without_written_numbers) {
		EXPECT_EQ("", day1_solution1::transform_written_numbers(""));
		EXPECT_EQ("abc", day1_solution1::transform_written_numbers("abc"));
		EXPECT_EQ("1", day1_solution1::transform_written_numbers("1"));
	}
	TEST(solution1, transforms_overlapping_numbers_each) {
		EXPECT_EQ("18", day1_solution1::transform_written_numbers("oneight"));
		EXPECT_EQ("79", day1_solution1::transform_written_numbers("sevenine"));
	}
	TEST(solution1, solution_part2_example2) {
		EXPECT_EQ(281, day1_solution1::handle_lines(day1_solution1::transformed_written_numbers(read_lines("./input/day1-example2.txt"))));
	}
	TEST(solution1, solution_part2) {
		EXPECT_EQ(55093, day1_solution1::handle_lines(day1_solution1::transformed_written_numbers(read_lines("./input/day1.txt"))));
	}
}
