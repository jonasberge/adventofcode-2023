#pragma once

#include <string>
#include <vector>
#include <array>
#include <utility>
#include <tuple>

namespace day7::solution1
{
	enum Card
	{
		Ace = 14,
		King = 13,
		Queen = 12,
		Jack = 11,
		Ten = 10,
		Nine = 9,
		Eight = 8,
		Seven = 7,
		Six = 6,
		Five = 5,
		Four = 4,
		Three = 3,
		Two = 2,
		Invalid = 0
	};

	enum HandType
	{
		FiveOfAKind = 7,
		FourOfAKind = 6,
		FullHouse = 5,
		ThreeOfAKind = 4,
		TwoPair = 3,
		OnePair = 2,
		HighCard = 1
	};

	struct Hand
	{
		static const size_t CardCount = 5;

		std::array<Card, CardCount> cards;

		HandType hand_type() const;
		std::pair<Card, Card> high_card(Hand const& other) const;

		friend bool operator==(Hand const&, Hand const&) = default;

		friend auto operator<=>(Hand const& a, Hand const& b)
		{
			auto ah = a.hand_type(), bh = b.hand_type();
			if (auto c = ah <=> bh; !std::is_eq(c)) return c;
			auto high = a.high_card(b);
			return high.first <=> high.second;
		}
	};

	struct Bid
	{
		Hand hand;
		size_t value;
	};

	struct Input
	{
		// parsed input data
		std::vector<Bid> bids;

		// functions that give solutions or solve parts
		size_t solve();
	};

	// input parsing functions
	Bid parse_line(std::string const& line);
	std::vector<Bid> parse_lines(std::vector<std::string> const& lines);
}
