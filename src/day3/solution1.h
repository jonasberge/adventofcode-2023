#pragma once

#include <string>
#include <vector>
#include <variant>
#include <utility>
#include <array>

namespace day3::solution1
{
	struct Position
	{
		int x;
		int y;

		inline int get_flat_index(int const width) const
		{
			return x + y * width;
		}

		Position() : x(0), y(0) {}
		Position(int x, int y) : x(x), y(y) {}

		friend bool operator==(Position const&, Position const&) = default;
	};

	// represents a value on the grid,
	// can be either a number of a symbol (not a period).
	struct Cell
	{
		// top-left position of the cell.
		Position position;
		// the value, either a number or a single character (symbol).
		std::variant<int, char> value;

		// returns the width of the cell, as in, the number of characters its value has.
		int get_width() const;

		Cell() {}
		Cell(Position position, std::variant<int, char> value) : position(position), value(value) {}

		friend bool operator==(Cell const&, Cell const&) = default;
	};

	struct Gear
	{
		Position position;
		std::array<int, 2> part_numbers;

		inline int gear_ratio() const
		{
			return part_numbers[0] * part_numbers[1];
		}

		friend bool operator==(Gear const& a, Gear const& b)
		{
			return a.position == b.position
				&& (a.part_numbers[0] == b.part_numbers[0] && a.part_numbers[1] == b.part_numbers[1]
					|| a.part_numbers[0] == b.part_numbers[1] && a.part_numbers[1] == b.part_numbers[0]);
		}
	};

	struct Schematic
	{
		int width;
		int height;
		std::vector<Cell> cells;

		std::vector<Cell> get_part_numbers() const;
		std::vector<Gear> get_gears() const;
	};

	struct Input
	{
		// parsed input data
		Schematic schematic;

		// functions that give solutions or solve parts
		int sum_of_part_numbers() const;
		int sum_of_gear_ratios() const;
	};

	// input parsing functions
	Schematic parse_lines(std::vector<std::string> const& lines);
}
