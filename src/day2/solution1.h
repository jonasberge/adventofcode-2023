#pragma once

#include <string>
#include <vector>

/// <summary>
/// Part 1:
/// - Cubes are either: red, green or blue
/// - Cubes are revealed and then put back into the bag
/// - Each line contains a single game, with its id and a number of revealed cube sets
/// - There is a number of maximally available cubes of each color, which renders some games impossible
/// </summary>
namespace day2::solution1
{
	struct CubeSet
	{
		int red;
		int green;
		int blue;

		bool is_less_than_or_equal(CubeSet const& other) const;
		long long power() const;

		CubeSet(): red(0), green(0), blue(0) {}
		CubeSet(int r, int g, int b): red(r), green(g), blue(b) {}

		bool operator==(const CubeSet& rhs) const
		{
			return red == rhs.red && green == rhs.green && blue == rhs.blue;
		}
	};

	struct Game
	{
		int id;
		std::vector<CubeSet> cube_sets;

		bool is_game_possible(CubeSet const& available_cubes) const;
		CubeSet minimum_set_of_cubes() const;

		bool operator==(const Game& rhs) const
		{
			return id == rhs.id && cube_sets == rhs.cube_sets;
		}
	};

	struct Input
	{
		CubeSet available_cubes;
		std::vector<Game> games;

		int sum_ids_of_possible_games() const;
		int sum_of_minimum_set_powers() const;
	};

	Game parse_line(std::string const& line);
	std::vector<Game> parse_lines(std::vector<std::string> const& lines);

	/*class Solution
	{
	public:
		virtual bool is_game_possible(Game const& game, CubeSet const& available_cubes) = 0;
		virtual int sum_ids_of_possible_games(std::vector<Game> const& games, CubeSet const& available_cubes) = 0;
	};

	class Solution1 : Solution
	{
		bool is_game_possible(Game const& game, CubeSet const& available_cubes) override;
		int sum_ids_of_possible_games(std::vector<Game> const& games, CubeSet const& available_cubes) override;
	};*/
}
