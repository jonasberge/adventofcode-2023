#pragma once

#include <string>
#include <vector>

namespace day9::solution1
{
	using int_t = long long;

	class PascalTriangle
	{
		std::vector<std::vector<size_t>> rows;

		void ensure_rows(size_t amount);

	public:
		const std::vector<size_t>& get_row(size_t index);
	};

	struct Sequence
	{
		std::vector<int_t> values;

		int_t extrapolate_value(PascalTriangle& pascal_triangle) const;
		int_t extrapolate_value_backwards(PascalTriangle& pascal_triangle) const;
	};

	struct Input
	{
		// parsed input data
		std::vector<Sequence> sequences;

		// functions that give solutions or solve parts
		int_t solve();
		int_t solve_backwards();
	};

	// input parsing functions
	Sequence parse_line(std::string const& line);
	std::vector<Sequence> parse_lines(std::vector<std::string> const& lines);
}
