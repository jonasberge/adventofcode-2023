#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <array>

namespace day8::solution1
{
	// left/right directions are indices 0 or 1 in the pair of network directions.
	using Index = int;
	using Direction = Index;

	const size_t DirectionSize = 2;

	using Network = std::unordered_map<std::string, std::array<std::string, DirectionSize>>;

	struct Map
	{
		std::vector<Direction> steps;
		Network network;
	};

	struct Input
	{
		// parsed input data
		Map map;

		// functions that give solutions or solve parts
		size_t solve();
	};

	// input parsing functions
	Map parse_lines(std::vector<std::string> const& lines);
}
