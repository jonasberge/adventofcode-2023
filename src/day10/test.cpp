#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "solution1.h"
#include "lib.h"

using namespace day10::solution1;
using namespace testing;

namespace day10
{
	TEST(Solution, Example1) {
		auto linesA = read_lines("./src/day10/input/example1a.txt");
		auto linesB = read_lines("./src/day10/input/example1b.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		Input inputB{ .grid = parse_lines(linesB) };
		auto solutionA = inputA.solve();
		auto solutionB = inputB.solve();
		ASSERT_EQ(solutionA, 4);
		ASSERT_EQ(solutionA, solutionB);
	}

	TEST(Solution, Example2) {
		auto linesA = read_lines("./src/day10/input/example2a.txt");
		auto linesB = read_lines("./src/day10/input/example2b.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		Input inputB{ .grid = parse_lines(linesB) };
		auto solutionA = inputA.solve();
		auto solutionB = inputB.solve();
		ASSERT_EQ(solutionA, 8);
		ASSERT_EQ(solutionA, solutionB);
	}

	TEST(Solution, Input) {
		auto linesA = read_lines("./src/day10/input/input.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		auto solutionA = inputA.solve();
		ASSERT_EQ(solutionA, 6867);
	}

	TEST(Solution, Part2Example1) {
		auto linesA = read_lines("./src/day10/input/part2example1.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		auto solutionA = inputA.solve_part2();
		ASSERT_EQ(solutionA, 4);
	}

	TEST(Solution, Part2Example2) {
		auto linesA = read_lines("./src/day10/input/part2example2.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		auto solutionA = inputA.solve_part2();
		ASSERT_EQ(solutionA, 4);
	}

	TEST(Solution, Part2Example3) {
		auto linesA = read_lines("./src/day10/input/part2example3.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		auto solutionA = inputA.solve_part2();
		ASSERT_EQ(solutionA, 8);
	}

	TEST(Solution, Part2Example4) {
		auto linesA = read_lines("./src/day10/input/part2example4.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		auto solutionA = inputA.solve_part2();
		ASSERT_EQ(solutionA, 10);
	}

	TEST(Solution, Part2Input) {
		auto linesA = read_lines("./src/day10/input/input.txt");
		Input inputA{ .grid = parse_lines(linesA) };
		auto solutionA = inputA.solve_part2();
		ASSERT_EQ(solutionA, 595);
	}
}
