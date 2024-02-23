#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>

#include "solution1.h"
#include "lib.h"

using namespace day4::solution1;
using namespace testing;

namespace day4
{
	class Tests : public testing::Test
	{
	protected:
		// add some spacing wider than once char, since that is in the large input as well.
		std::string card1_input = "Card   1: 41 48   83 86 17 | 83 86  6 31 17  9 48 53";
		std::string card2_input = "Card 2: 13 32 20 16 61 |  61 30   68 82 17 32 24 19";

		Card card1 = Card{ 1, { 41, 48, 83, 86, 17 }, { 83, 86, 6, 31, 17, 9, 48, 53 } };
		Card card2 = Card{ 2, { 13, 32, 20, 16, 61 }, { 61, 30, 68, 82, 17, 32, 24, 19 } };
	};

	TEST_F(Tests, ParsesLineProperly) {
		EXPECT_EQ(card1, parse_line(card1_input));
		EXPECT_EQ(card2, parse_line(card2_input));
	}
	TEST_F(Tests, WinningNumbers) {
		EXPECT_THAT(card1.determine_own_winning_numbers(), UnorderedElementsAre(48, 83, 17, 86));
		EXPECT_THAT(card2.determine_own_winning_numbers(), UnorderedElementsAre(32, 61));
	}
	TEST_F(Tests, PointValues) {
		EXPECT_EQ(8, card1.get_point_value());
		EXPECT_EQ(2, card2.get_point_value());
	}
	TEST_F(Tests, SumPointValues) {
		Input input{ .cards = { card1, card2 } };
		EXPECT_EQ(10, input.sum_point_values());
	}

	// TODO: generalize this?
	std::vector<Card> read_cards(std::string const& filename)
	{
		return parse_lines(read_lines("./day4/input/" + filename + ".txt"));
	}

	class InputTests : public testing::Test
	{
	protected:
		Input get_input(std::string filename)
		{
			return Input{ .cards = read_cards(filename) };
		}
	};

	TEST_F(InputTests, Example1) {
		Input input = get_input("example1");
		EXPECT_THAT(input.cards[0].determine_own_winning_numbers(), UnorderedElementsAre(48, 83, 17, 86));
		EXPECT_THAT(input.cards[1].determine_own_winning_numbers(), UnorderedElementsAre(32, 61));
		EXPECT_THAT(input.cards[2].determine_own_winning_numbers(), UnorderedElementsAre(1, 21));
		EXPECT_THAT(input.cards[3].determine_own_winning_numbers(), UnorderedElementsAre(84));
		EXPECT_THAT(input.cards[4].determine_own_winning_numbers(), UnorderedElementsAre());
		EXPECT_THAT(input.cards[5].determine_own_winning_numbers(), UnorderedElementsAre());
		EXPECT_EQ(8, input.cards[0].get_point_value());
		EXPECT_EQ(2, input.cards[1].get_point_value());
		EXPECT_EQ(2, input.cards[2].get_point_value());
		EXPECT_EQ(1, input.cards[3].get_point_value());
		EXPECT_EQ(0, input.cards[4].get_point_value());
		EXPECT_EQ(0, input.cards[5].get_point_value());
		EXPECT_EQ(13, input.sum_point_values());
		EXPECT_EQ(30, input.sum_real_card_count());
	}
	TEST_F(InputTests, Solution) {
		Input input = get_input("input");
		EXPECT_EQ(18619, input.sum_point_values());
		EXPECT_EQ(8063216, input.sum_real_card_count());
	}
}
