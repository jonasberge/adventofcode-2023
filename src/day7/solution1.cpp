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
		_ASSERT(value >= Card::Two);
		_ASSERT(value <= Card::Ace);
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

HandType day7::solution1::Hand::hand_type() const
{
	std::array<int, Card::Ace + 1> counts = {};
	for (Card card : cards) {
		counts[card] += 1;
	}
	std::array<int, Hand::CardCount + 1> inverse = {};
	for (int i = Card::Two; i <= Card::Ace; i++) {
		Card card = static_cast<Card>(i);
		int count = counts[i];
		inverse[count]++;
	}
	if (inverse[5] == 1) return HandType::FiveOfAKind;
	if (inverse[4] == 1) return HandType::FourOfAKind;
	if (inverse[3] == 1 && inverse[2] == 1) return HandType::FullHouse;
	if (inverse[3] == 1) return HandType::ThreeOfAKind;
	if (inverse[2] == 2) return HandType::TwoPair;
	if (inverse[2] == 1) return HandType::OnePair;
	return HandType::HighCard;
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
