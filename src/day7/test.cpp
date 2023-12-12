#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "solution1.h"
#include "lib.h"

using namespace day7::solution1;
using namespace testing;

namespace day7
{
	static Hand parse_hand(std::string const& hand) {
		return parse_line(hand + " 0").hand;
	}

	TEST(ParseLines, ParseLine) {
		Bid bid = parse_line("AK352 1222");
		EXPECT_THAT(bid.hand.cards, UnorderedElementsAre(Card::Ace, Card::King, Card::Three, Card::Five, Card::Two));
		EXPECT_EQ(1222, bid.value);
	}
	TEST(Hand, HandType) {
		EXPECT_EQ(HandType::FiveOfAKind, parse_hand("TTTTT").hand_type());
		EXPECT_EQ(HandType::FourOfAKind, parse_hand("TTTT2").hand_type());
		EXPECT_EQ(HandType::FourOfAKind, parse_hand("TT2TT").hand_type());
		EXPECT_EQ(HandType::FullHouse, parse_hand("33322").hand_type());
		EXPECT_EQ(HandType::ThreeOfAKind, parse_hand("33324").hand_type());
		EXPECT_EQ(HandType::TwoPair, parse_hand("AAKK2").hand_type());
		EXPECT_EQ(HandType::OnePair, parse_hand("2AA45").hand_type());
		EXPECT_EQ(HandType::HighCard, parse_hand("A52K9").hand_type());
	}
	TEST(Hand, Ordering) {
		EXPECT_GT(parse_hand("AAAAA"), parse_hand("AAAAK"));
		EXPECT_GT(parse_hand("TTT98"), parse_hand("TT325"));
	}
	TEST(Hand, OrderingTiedHighCard) {
		EXPECT_GT(parse_hand("AAAA9"), parse_hand("AAAA8"));
		EXPECT_GT(parse_hand("33332"), parse_hand("2AAAA"));
		EXPECT_GT(parse_hand("77888"), parse_hand("77788"));
	}
	TEST(Solution, Example1) {
		auto lines = read_lines("./src/day7/input/example1.txt");
		Input input{ .bids = parse_lines(lines) };
		EXPECT_EQ(6440, input.solve());
	}
	TEST(Solution, Input) {
		auto lines = read_lines("./src/day7/input/input.txt");
		Input input{ .bids = parse_lines(lines) };
		EXPECT_EQ(246409899, input.solve());
	}
}
