#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "solution1.h"
#include "lib.h"

using namespace day3::solution1;
using namespace testing;

namespace day3
{
	class Tests : public Test
	{
	protected:
		std::vector<std::string> schematic1_text = read_lines("./day03/input/example1.txt");
		Schematic schematic1 = {
			.width = 10,
			.height = 10,
			.cells = {
				{ { 0, 0 }, { 467 } },
				{ { 5, 0 }, { 114 } },
				{ { 3, 1 }, { '*' }},
				{ { 2, 2 }, { 35 }},
				{ { 6, 2 }, { 633 }},
				{ { 6, 3 }, { '#'}},
				{ { 0, 4 }, { 617 }},
				{ { 3, 4 }, { '*' }},
				{ { 5, 5 }, { '+' }},
				{ { 7, 5 }, { 58 }},
				{ { 2, 6 }, { 592 }},
				{ { 6, 7 }, { 755 }},
				{ { 3, 8 }, { '$' }},
				{ { 5, 8 }, { '*' }},
				{ { 1, 9 }, { 664 }},
				{ { 5, 9 }, { 598 }},
			}
		};
		std::vector<Cell> schematic1_part_numbers = {
			{ { 0, 0 }, { 467 } },
			{ { 2, 2 }, { 35 }},
			{ { 6, 2 }, { 633 }},
			{ { 0, 4 }, { 617 }},
			{ { 2, 6 }, { 592 }},
			{ { 6, 7 }, { 755 }},
			{ { 1, 9 }, { 664 }},
			{ { 5, 9 }, { 598 }},
		};
		Gear gear1 = Gear{ .position = { 5, 8 }, .part_numbers = { 755, 598 } };
		Gear gear2 = Gear{ .position = { 3, 1 }, .part_numbers = { 467, 35 } };
	};

	TEST_F(Tests, ParseLines) {
		EXPECT_THAT(parse_lines(schematic1_text).cells, UnorderedElementsAreArray(schematic1.cells));
	}
	TEST_F(Tests, PartNumbers) {
		EXPECT_THAT(schematic1.get_part_numbers(), UnorderedElementsAreArray(schematic1_part_numbers));
		// add a symbol that is in the corner and next to a number, which wasn't a part number before.
		schematic1.cells.push_back(Cell(Position(8, 0), '*'));
		schematic1_part_numbers.push_back({ { 5, 0 }, { 114 } });
		EXPECT_THAT(schematic1.get_part_numbers(), UnorderedElementsAreArray(schematic1_part_numbers));
	}
	TEST_F(Tests, Gears) {
		std::vector<Gear> expected = { gear1, gear2 };
		EXPECT_THAT(schematic1.get_gears(), UnorderedElementsAreArray(expected));
	}
	TEST_F(Tests, GearRatios) {
		auto gears = schematic1.get_gears();
		std::vector<int> expected = { 16345, 451490 };
		std::vector<int> actual = { gears[0].gear_ratio(), gears[1].gear_ratio() };
		EXPECT_THAT(actual, UnorderedElementsAreArray(expected));
	}

	// TODO: generalize this?
	Schematic read_schematic(std::string const& filename)
	{
		auto lines = read_lines("./day03/input/" + filename + ".txt");
		return parse_lines(lines);
	}

	class InputTests : public testing::Test
	{
	protected:
		Input get_input(std::string filename)
		{
			return Input{ .schematic = read_schematic(filename) };
		}
	};

	TEST_F(InputTests, Example1) {
		Input input = get_input("example1");
		EXPECT_EQ(4361, input.sum_of_part_numbers());
		// part 2
		EXPECT_EQ(467835, input.sum_of_gear_ratios());
	}
	TEST_F(InputTests, Solution) {
		Input input = get_input("input");
		EXPECT_EQ(517021, input.sum_of_part_numbers());
		// part 2
		EXPECT_EQ(81296995, input.sum_of_gear_ratios());
	}
}
