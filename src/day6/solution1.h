#pragma once

#include <string>
#include <vector>

namespace day6::solution1
{
	struct Race
	{
		size_t time;
		size_t distance;

		friend bool operator==(Race const&, Race const&) = default;
	};

	struct Input
	{
		// parsed input data
		std::vector<Race> races;

		// functions that give solutions or solve parts
		size_t solve();
	};

	// input parsing functions
	std::vector<size_t> parse_line(std::string const& line);
	std::vector<Race> parse_lines(std::vector<std::string> const& lines);
}
