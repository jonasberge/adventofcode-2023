#include <gtest/gtest.h>
#include <utility>
#include <algorithm>

#include "solution1.h"
#include "lib.h"

using namespace day2::solution1;

namespace day2
{
	TEST(CubeSetTests, IsLessThanOrEqual) {
		EXPECT_TRUE(CubeSet(0, 0, 0).is_less_than_or_equal(CubeSet(0, 0, 0)));
		EXPECT_TRUE(CubeSet(0, 0, 0).is_less_than_or_equal(CubeSet(1, 0, 0)));
		EXPECT_TRUE(CubeSet(0, 0, 0).is_less_than_or_equal(CubeSet(1, 1, 1)));
	}
	TEST(CubeSetTests, HasOneLargerElement) {
		EXPECT_FALSE(CubeSet(1, 0, 0).is_less_than_or_equal(CubeSet(0, 0, 0)));
		EXPECT_FALSE(CubeSet(0, 1, 0).is_less_than_or_equal(CubeSet(1, 0, 0)));
		EXPECT_FALSE(CubeSet(0, 0, 2).is_less_than_or_equal(CubeSet(1, 1, 1)));
	}

	class Tests : public testing::Test
	{
	protected:
		std::string game1_input = "Game 13: 3 blue, 4 red; 1000 red, 2 green, 6 blue; 21 green";
		std::string game2_input = "Game 5: 6 red, 1 blue, 3 green; 2 blue, 1 red, 2 green";

		Game game1 = Game{ 13, { CubeSet{ 4, 0, 3 }, CubeSet{ 1000, 2, 6 }, CubeSet{ 0, 21, 0 } } };
		Game game2 = Game{ 5, { CubeSet{ 6, 3, 1 }, CubeSet{ 1, 2, 2 } } };
	};

	TEST_F(Tests, ParsesLineProperly) {
		EXPECT_EQ(game1, parse_line(game1_input));
		EXPECT_EQ(game2, parse_line(game2_input));
	}
	TEST_F(Tests, IsGamePossible) {
		EXPECT_TRUE(game1.is_game_possible(CubeSet(1000, 21, 6)));
		EXPECT_FALSE(game1.is_game_possible(CubeSet(999, 21, 6)));
		EXPECT_FALSE(game1.is_game_possible(CubeSet(1000, 20, 6)));
		EXPECT_FALSE(game1.is_game_possible(CubeSet(1000, 21, 5)));
	}
	TEST_F(Tests, SumsIdsOfPossibleGames) {
		Input input_no_game{ CubeSet(1, 1, 1), { game1, game2 } };
		Input input_game2{ CubeSet(999, 21, 6), { game1, game2 } };
		Input input_game12{ CubeSet(1000, 21, 6), { game1, game2 } };
		EXPECT_EQ(0, input_no_game.sum_ids_of_possible_games());
		EXPECT_EQ(game2.id, input_game2.sum_ids_of_possible_games());
		EXPECT_EQ(game1.id + game2.id, input_game12.sum_ids_of_possible_games());
	}
	// part 2
	TEST_F(Tests, MinimumSetOfCubes) {
		EXPECT_EQ(CubeSet(1000, 21, 6), game1.minimum_set_of_cubes());
		EXPECT_EQ(CubeSet(6, 3, 2), game2.minimum_set_of_cubes());
	}
	TEST_F(Tests, CubeSetPower) {
		// TODO
	}

	std::vector<Game> read_games(std::string const& filename)
	{
		auto lines = read_lines("./day02/input/" + filename + ".txt");
		return parse_lines(lines);
	}

	class InputTests : public testing::Test
	{
	protected:
		// constraints: "12 red cubes, 13 green cubes, and 14 blue cubes"
		CubeSet constraints = CubeSet(12, 13, 14);

		Input get_input(std::string filename)
		{
			return Input{ constraints, read_games(filename) };
		}
	};

	TEST_F(InputTests, Example1) {
		Input input = get_input("example1");
		EXPECT_EQ(8, input.sum_ids_of_possible_games());
		// part 2: minimum set of cubes
		std::vector<CubeSet> minimum_cube_sets(input.games.size());
		std::transform(input.games.begin(), input.games.end(), minimum_cube_sets.begin(),
			[](Game const& game) {
				return game.minimum_set_of_cubes();
			});
		EXPECT_EQ(CubeSet(4, 2, 6), minimum_cube_sets[0]);
		EXPECT_EQ(CubeSet(1, 3, 4), minimum_cube_sets[1]);
		EXPECT_EQ(CubeSet(20, 13, 6), minimum_cube_sets[2]);
		EXPECT_EQ(CubeSet(14, 3, 15), minimum_cube_sets[3]);
		EXPECT_EQ(CubeSet(6, 3, 2), minimum_cube_sets[4]);
		// powers
		EXPECT_EQ(48, minimum_cube_sets[0].power());
		EXPECT_EQ(12, minimum_cube_sets[1].power());
		EXPECT_EQ(1560, minimum_cube_sets[2].power());
		EXPECT_EQ(630, minimum_cube_sets[3].power());
		EXPECT_EQ(36, minimum_cube_sets[4].power());
		// solution part 2
		EXPECT_EQ(2286, input.sum_of_minimum_set_powers());
	}
	TEST_F(InputTests, Solution) {
		Input input = get_input("input");
		EXPECT_EQ(2563, input.sum_ids_of_possible_games());
		// part 2
		EXPECT_EQ(70768, input.sum_of_minimum_set_powers());
	}
}
