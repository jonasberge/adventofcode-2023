#pragma once

#include <string>
#include <vector>
#include <unordered_set>

namespace day4::solution1
{
	struct Card
	{
		int id;
		std::unordered_set<int> winning_numbers;
		std::unordered_set<int> own_numbers;

		std::unordered_set<int> determine_own_winning_numbers() const;
		int get_point_value() const;

		friend bool operator==(Card const&, Card const&) = default;
	};

	struct Input
	{
		// parsed input data
		std::vector<Card> cards;

		// functions that give solutions or solve parts
		int sum_point_values();
		int sum_real_card_count();
	};

	// input parsing functions
	Card parse_line(std::string const& line);
	std::vector<Card> parse_lines(std::vector<std::string> const& lines);
}
