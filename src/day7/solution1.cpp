#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <array>
#include <utility>

#include "solution1.h"

using namespace day7::solution1;

Bid day7::solution1::parse_line(std::string const& line)
{
	std::istringstream iss(line);
	Hand hand;
	for (int i = 0; i < 5; i++) {
		char c;
		iss >> c;
		int value = 0;
		switch (c) {
		// special symbol for joker
		case '?': value = Card::Joker; break;
		case 'A': value = Card::Ace; break;
		case 'K': value = Card::King; break;
		case 'Q': value = Card::Queen; break;
		case 'J': value = Card::Jack; break;
		case 'T': value = 10; break;
		default:
			if (std::isdigit(c)) {
				value = c - '0';
			}
		}
		_ASSERT(value >= Card::LowestCard);
		_ASSERT(value <= Card::HighestCard);
		hand.cards[i] = static_cast<Card>(value);
	}
	size_t bid;
	iss >> bid;
	return Bid{
		.hand = hand,
		.value = bid
	};
}

std::vector<Bid> day7::solution1::parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Bid> bids(lines.size());
	std::transform(lines.begin(), lines.end(), bids.begin(), ::parse_line);
	return bids;
}

void day7::solution1::Input::apply_jokers()
{
	for (Bid& bid : bids) {
		for (size_t i = 0; i < bid.hand.cards.size(); i++) {
			if (bid.hand.cards[i] == Card::Jack) {
				bid.hand.cards[i] = Card::Joker;
			}
		}
	}
}

HandType day7::solution1::Hand::hand_type() const
{
	std::array<int, Card::Ace + 1> counts = {};
	size_t joker_count = 0;
	for (Card card : cards) {
		if (card == Card::Joker) {
			joker_count++;
		}
		else {
			// only increment if not a joker
			counts[card]++;
		}
	}
	// inverse table, to look up how many counts there are for a specific amount.
	std::array<int, Hand::CardCount + 1> inverse = {};
	for (int i = Card::Two; i <= Card::Ace; i++) {
		Card card = static_cast<Card>(i);
		int count = counts[i];
		inverse[count]++;
	}

	HandType hand_type = HighCard;
	if (inverse[5] == 1) hand_type = FiveOfAKind;
	else if (inverse[4] == 1) hand_type = FourOfAKind;
	else if (inverse[3] == 1 && inverse[2] == 1) hand_type = FullHouse;
	else if (inverse[3] == 1) hand_type = ThreeOfAKind;
	else if (inverse[2] == 2) hand_type = TwoPair;
	else if (inverse[2] == 1) hand_type = OnePair;

	// converts a joker_count+hand_type pair to an integral value
	// which can be used in a switch statement.
	static constexpr auto _ = [](size_t joker_count, HandType hand_type)
		{
			// maximum value is 5 => max. exponent 8 = 2^(3)
			const size_t MAX_JOKER_COUNT_EXPONENT = 3;
			_ASSERT((joker_count >> MAX_JOKER_COUNT_EXPONENT) == 0);
			return (size_t(hand_type) << MAX_JOKER_COUNT_EXPONENT) + joker_count;
		};

	const auto DEFAULT = HighCard;

	switch (_(joker_count, hand_type)) {
	case _(5, DEFAULT): return FiveOfAKind;
	case _(4, DEFAULT): return FiveOfAKind;
	case _(3, OnePair): return FiveOfAKind;
	case _(3, DEFAULT): return FourOfAKind;
	case _(2, ThreeOfAKind): return FiveOfAKind;
	case _(2, OnePair): return FourOfAKind;
	case _(2, DEFAULT): return ThreeOfAKind;
	case _(1, FourOfAKind): return FiveOfAKind;
	case _(1, ThreeOfAKind): return FourOfAKind;
	case _(1, TwoPair): return FullHouse;
	case _(1, OnePair): return ThreeOfAKind;
	case _(1, DEFAULT): return OnePair;
	default: return hand_type;
	}

	// enumerating all possibilities in the switch statement above:
	// 4 jokers:
	// - only possible: high card
	// - if has high card (default): makes 5 of a kind
	// 3 jokers:
	// - only possible: one pair, high card
	// - if has one pair: makes 5 of a kind
	// - default: makes 4 of a kind
	// 2 jokers:
	// - not possible with other 3 cards: 5 of a kind, 4 of a kind, full house, 2 pair
	// - if has 3 of a kind: makes 5 of a kind
	// - if has 1 pair: makes 4 of a kind
	// - default: makes 3 of a kind
	// 1 joker:
	// - not possible with other 4 cards: 5 of a kind, full house
	// - if has 4 of a kind: makes 5 of a kind
	// - if has 3 of a kind: makes 4 of a kind
	// - if has two pair: makes full house
	// - if has one pair: makes three of a kind
	// - default: makes one pair
}

std::pair<Card, Card> day7::solution1::Hand::high_card(Hand const& other) const
{
	for (int i = 0; i < cards.size(); i++) {
		if (cards[i] != other.cards[i]) {
			return std::make_pair(cards[i], other.cards[i]);
		}
	}
	return std::make_pair(cards[cards.size() - 1], other.cards[other.cards.size() - 1]);
}

size_t day7::solution1::Input::solve()
{
	std::vector<Bid> sorted_bids(bids);
	std::sort(sorted_bids.begin(), sorted_bids.end(),
		[](Bid const& a, Bid const& b) {
			return a.hand < b.hand;
		});

	size_t sum = 0;
	for (size_t i = 0; i < sorted_bids.size(); i++) {
		Bid const& bid = sorted_bids[i];
		size_t rank = i + 1;
		sum += bid.value * rank;
	}

	return sum;
}
