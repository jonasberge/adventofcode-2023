#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>
#include <ranges>
#include <algorithm>
#include <numeric>

#include "solution1.h"

using namespace day4::solution1;

Card day4::solution1::parse_line(std::string const& line)
{
	Card card{};

	std::istringstream input(line);
	std::string segment;
	std::getline(input, segment, ':');
	card.id = std::stoi(segment.substr(segment.find_last_of(' ') + 1));

	std::unordered_set<int>* ref = &card.winning_numbers;

	while (std::getline(input, segment, ' ')) {
		if (segment.size() == 0) {
			continue;
		}
		if (segment.starts_with('|')) {
			ref = &card.own_numbers;
			continue;
		}
		int value = std::stoi(segment);
		ref->insert(value);
	}

	return card;
}

std::vector<Card> day4::solution1::parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Card> cards(lines.size());
	std::transform(lines.begin(), lines.end(), cards.begin(), ::parse_line);
	return cards;
}

std::unordered_set<int> Card::determine_own_winning_numbers() const
{
	auto view = own_numbers | std::views::filter([this](int e) {
		return winning_numbers.contains(e);
	});
	return std::unordered_set<int>(view.begin(), view.end());
}

int Card::get_point_value() const
{
	int count = determine_own_winning_numbers().size();
	return count <= 0 ? 0 : (1 << (count - 1));
}

int Input::sum_point_values()
{
	int sum = 0;
	for (Card const& card : cards) {
		sum += card.get_point_value();
	}
	return sum;
}

int Input::sum_real_card_count()
{
	std::vector<int> card_counts(cards.size(), 1);
	int offset = 0;
	int sum = 0;

	for (Card const& card : cards) {
		auto numbers = card.determine_own_winning_numbers();
		for (int i = 0; i < numbers.size(); i++) {
			card_counts[card.id + i] += card_counts[offset];
		}
		sum += card_counts[offset++];
	}

	return sum;
}
