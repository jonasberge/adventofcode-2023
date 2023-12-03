#include <iostream>
#include <regex>
#include <vector>
#include <sstream>
#include <string>
#include <cmath>
#include <algorithm>
#include <functional>

#include "solution1.h"

using namespace day2::solution1;
using namespace std::string_literals;

Game day2::solution1::parse_line(std::string const& line)
{
	Game game{};

	std::istringstream input(line);
	std::string segment;
	std::getline(input, segment, ':');
	// the game id is always after the "Game " prefix.
	game.id = std::stoi(segment.substr("Game "s.length()));

	while (std::getline(input, segment, ';')) {
		std::istringstream revealed_cubes(segment);
		std::string data;
		CubeSet cube_set{};
		while (std::getline(revealed_cubes, data, ',')) {
			// skip the space after each separator.
			data = data.substr(1);
			// determine the cube count and how many digits it has.
			int cube_count = std::stoi(data);
			std::size_t cube_count_digits = (cube_count > 0 ? std::floorl(std::log10(cube_count)) : 0) + 1;
			// skip the cube count and the space after it.
			data = data.substr(cube_count_digits + 1);
			// increment the respective cube set field.
			if (data == "red") cube_set.red += cube_count;
			else if (data == "green") cube_set.green += cube_count;
			else if (data == "blue") cube_set.blue += cube_count;
		}
		game.cube_sets.push_back(cube_set);
	}

	return game;
}

std::vector<Game> day2::solution1::parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Game> games(lines.size());
	std::transform(lines.begin(), lines.end(), games.begin(), ::parse_line);
	return games;
}

long long CubeSet::power() const
{
	return red * green * blue;
}

bool CubeSet::is_less_than_or_equal(CubeSet const& other) const
{
	return this->red <= other.red && this->green <= other.green && this->blue <= other.blue;
}

bool Game::is_game_possible(CubeSet const& available_cubes) const
{
	for (CubeSet const& cube_set : this->cube_sets) {
		if (!cube_set.is_less_than_or_equal(available_cubes)) {
			return false;
		}
	}
	return true;
}

CubeSet Game::minimum_set_of_cubes() const
{
	CubeSet minimum;
	for (CubeSet const& cube_set : this->cube_sets) {
		minimum.red = std::max(minimum.red, cube_set.red);
		minimum.green = std::max(minimum.green, cube_set.green);
		minimum.blue = std::max(minimum.blue, cube_set.blue);
	}
	return minimum;
}

int Input::sum_ids_of_possible_games() const
{
	int sum = 0;
	for (Game const& game : this->games) {
		if (game.is_game_possible(this->available_cubes)) {
			sum += game.id;
		}
	}
	return sum;
}

int Input::sum_of_minimum_set_powers() const
{
	int sum = 0;
	for (Game const& game : this->games) {
		sum += game.minimum_set_of_cubes().power();
	}
	return sum;
}
