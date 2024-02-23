#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <optional>
#include <array>
#include <algorithm>
#include <unordered_set>
#include <functional>
#include <iostream>

#include "solution1.h"

using namespace day10::solution1;

std::vector<Pipe> day10::solution1::parse_line(std::string const& line)
{
	std::istringstream iss(line);
	char c;
	std::vector<Pipe> result;
	while (iss >> c) {
		switch (c) {
		case '|': result.push_back(Pipe({ Direction::North, Direction::South })); break;
		case '-': result.push_back(Pipe({ Direction::West, Direction::East })); break;
		case 'L': result.push_back(Pipe({ Direction::North, Direction::East })); break;
		case 'J': result.push_back(Pipe({ Direction::North, Direction::West })); break;
		case '7': result.push_back(Pipe({ Direction::West, Direction::South })); break;
		case 'F': result.push_back(Pipe({ Direction::East, Direction::South })); break;
		case 'S': result.push_back(Pipe(PIPE_SIDES_START)); break;
		case '.': result.push_back(Pipe(PIPE_SIDES_NONE)); break;
		default: throw "unexpected input element";
		}
	}
	return result;
}

std::vector<std::vector<Pipe>> day10::solution1::parse_lines(std::vector<std::string> const& lines)
{
	std::vector<std::vector<Pipe>> result(lines.size());
	std::transform(lines.begin(), lines.end(), result.begin(), ::parse_line);
	return result;
}

Field day10::solution1::Input::parse_grid()
{
	Field field(grid.size(), grid[0].size());

	// start position
	for (size_t y = 0; y < grid.size(); y++) {
		for (size_t x = 0; x < grid[y].size(); x++) {
			auto const& element = grid[y][x];
			if (element.sides == PIPE_SIDES_START) {
				field.start_y = y;
				field.start_x = x;
			}
		}
	}

	auto sx = field.start_x;
	auto sy = field.start_y;

	for (size_t y = 0; y < grid.size(); y++) {
		for (size_t x = 0; x < grid[y].size(); x++) {
			auto const& element = grid[y][x];
			if (element.sides == PIPE_SIDES_NONE) {
				continue;
			}
			std::unordered_set<Direction> directions({ element.sides.first, element.sides.second });
			if (element.sides == PIPE_SIDES_START) {
				// this is the starting position, so consider all directions where we could go to.
				directions = { Direction::North, Direction::East, Direction::South, Direction::West };
			}
			_ASSERT(!directions.contains(Direction::Unknown));
			_ASSERT(!directions.contains(Direction::None));

			std::unordered_set<Direction> filtered;
			filtered.reserve(directions.size());
			for (Direction direction : directions) {
				bool neighbour_points = false;
				switch (direction) {
				case Direction::North: neighbour_points = y > 0 && grid[y - 1][x].contains(Direction::South) || y - 1 == sy && x == sx; break;
				case Direction::West:  neighbour_points = x > 0 && grid[y][x - 1].contains(Direction::East) || y == sy && x - 1 == sx; break;
				case Direction::East:  neighbour_points = x + 1 < grid[y].size() && grid[y][x + 1].contains(Direction::West) || y == sy && x + 1 == sx; break;
				case Direction::South: neighbour_points = y + 1 < grid.size() && grid[y + 1][x].contains(Direction::North) || y + 1 == sy && x == sx; break;
				}
				if (neighbour_points) {
					filtered.insert(direction);
				}
			}
			std::vector<Direction> filtered_vector(filtered.begin(), filtered.end());
			field.directions_grid[y][x].swap(filtered_vector);
		}
	}

	return field;
}

void day10::solution1::Field::iterate_both_directions(std::function<void(const Vector&, const Vector&)> callback)
{
	Vector v1(start_x, start_y), v2(start_x, start_y);

	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

	visited[start_y][start_x] = true;
	callback(v1, v2);

	while (true) {

		auto& d1 = directions_grid[v1.y][v1.x];
		auto& d2 = directions_grid[v2.y][v2.x];
		bool has_moved = false;

		for (size_t i = 1; i <= 2; i++) {
			auto& directions = i == 1 ? d1 : d2;
			for (Direction direction : directions) {
				auto ov = i == 1 ? v1 : v2;
				auto v = ov.plus(Vector::for_direction(direction));
				if (v.y < 0 || v.y >= visited.size() || v.x < 0 || v.x >= visited[v.y].size())
					continue;
				if (visited[v.y][v.x])
					continue;
				has_moved = true;
				visited[v.y][v.x] = true;
				if (i == 1) {
					v1 = v;
				}
				else {
					v2 = v;
				}
				break;
			}
		}

		if (!has_moved)
			break;

		callback(v1, v2);
	}
}

struct DirectionPairHash
{
	size_t operator()(const std::pair<Direction, Direction>& x) const
	{
		return static_cast<size_t>(x.first) + 10 * static_cast<size_t>(x.second);
	}
};

void day10::solution1::Field::iterate_single_direction(size_t index, std::function<void(const Vector&, Direction, Turn)> callback)
{
	static std::unordered_set<std::pair<Direction, Direction>, DirectionPairHash> right_turns = {
		{ Direction::North, Direction::East },
		{ Direction::East, Direction::South },
		{ Direction::South, Direction::West },
		{ Direction::West, Direction::North },
	};
	static std::unordered_set<std::pair<Direction, Direction>, DirectionPairHash> left_turns = {
		{ Direction::North, Direction::West },
		{ Direction::West, Direction::South },
		{ Direction::South, Direction::East },
		{ Direction::East, Direction::North },
	};

	Vector v1(start_x, start_y);

	std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));

	visited[start_y][start_x] = true;

	bool picked_start = false;
	Direction previous_direction = Direction::None;

	while (true) {

		bool has_moved = false;

		auto& grid_directions = directions_grid[v1.y][v1.x];
		std::vector<Direction> directions;

		if (!picked_start) {
			directions.push_back(grid_directions[index]);
			picked_start = true;
		}
		else {
			directions.insert(directions.begin(), grid_directions.begin(), grid_directions.end());
		}

		auto old_v1 = v1;
		Direction target_direction = Direction::None;

		for (Direction direction : directions) {
			auto v = v1.plus(Vector::for_direction(direction));
			if (v.y < 0 || v.y >= visited.size() || v.x < 0 || v.x >= visited[v.y].size())
				continue;
			if (v.x == start_x && v.y == start_y)
				// set the direction if we go back to the starting position
				target_direction = direction;
			if (visited[v.y][v.x])
				continue;
			has_moved = true;
			visited[v.y][v.x] = true;
			v1 = v;
			target_direction = direction;
			break;
		}

		Turn turn = Turn::None;
		if (previous_direction != Direction::None) {
			auto turn_direction = std::make_pair(previous_direction, target_direction);
			if (right_turns.contains(turn_direction)) {
				turn = Turn::RightTurn;
			}
			if (left_turns.contains(turn_direction)) {
				turn = Turn::LeftTurn;
			}
		}

		callback(old_v1, target_direction, turn);

		if (!has_moved)
			break;

		previous_direction = target_direction;
	}
}

size_t day10::solution1::Input::solve()
{
	size_t step_count(0);

	Field field = parse_grid();
	field.iterate_both_directions(
		[&step_count](const Vector& v1, const Vector& v2) {
			step_count += 1;
		});

	return step_count - 1;
}

static Direction turn_direction(Direction d, bool isRight)
{
	Direction d2 = Direction::None;
	if (isRight) {
		switch (d) {
		case Direction::North: d2 = Direction::East; break;
		case Direction::East: d2 = Direction::South; break;
		case Direction::South: d2 = Direction::West; break;
		case Direction::West: d2 = Direction::North; break;
		}
	}
	else {
		switch (d) {
		case Direction::North: d2 = Direction::West; break;
		case Direction::West: d2 = Direction::South; break;
		case Direction::South: d2 = Direction::East; break;
		case Direction::East: d2 = Direction::North; break;
		}
	}
	return d2;
}

size_t day10::solution1::Input::solve_part2()
{
	static const size_t index = 0;
	int turns = 0;

	// find out which side of the direction at index 0 is the "inside" of the loop.
	// we will place our "inside" markers on that side.
	// if "turns" is positive, then the "inside" is on the right,
	// otherwise it's on the left of the pipe.
	Field field = parse_grid();
	field.iterate_single_direction(index,
		[&turns](const Vector& v, Direction d, Turn t) {
			switch (t) {
			case Turn::RightTurn: turns++; break;
			case Turn::LeftTurn: turns--; break;
			}
		});

	std::vector<std::vector<bool>> enclosed(field.height, std::vector<bool>(field.width, false));

	// mark initial spots that are on the inside of the loop.
	// these aren't all spots, since some areas can be large and need to be filled out.
	field.iterate_single_direction(index,
		[&field, &turns, &enclosed](const Vector& v, Direction d, Turn t) {
			Direction nd = turn_direction(d, turns >= 0);
			Vector n1 = v.plus(Vector::for_direction(nd));
			if (n1.x >= 0 && n1.x < field.width && n1.y >= 0 && n1.y < field.height) {
				enclosed[n1.y][n1.x] = true;
			}
			if (t != Turn::None) {
				// we want to find out the direction we are coming from, so it's a left turn.
				Direction od = turn_direction(d, t != Turn::RightTurn);
				// then take the proper neighbour of that as well and add it.
				Direction nd2 = turn_direction(od, turns >= 0);
				Vector n2 = v.plus(Vector::for_direction(nd2));
				if (n2.x >= 0 && n2.x < field.width && n2.y >= 0 && n2.y < field.height) {
					enclosed[n2.y][n2.x] = true;
				}
			}
		});

	std::vector<std::vector<bool>> visited(field.height, std::vector<bool>(field.width, false));

	field.iterate_single_direction(index,
		[&visited, &enclosed](const Vector& v, Direction d, Turn t) {
			// make sure visited pipes/spots are not considered "inside" the loop.
			enclosed[v.y][v.x] = false;
			visited[v.y][v.x] = true;
		});

	// now expand/spread out each "inside" marker.
	std::vector<Vector> coords;
	for (size_t y = 0; y < field.height; y++) {
		for (size_t x = 0; x < field.width; x++) {
			if (enclosed[y][x]) {
				coords.push_back(Vector(x, y));
			}
		}
	}
	while (coords.size() > 0) {
		auto& v = coords.back();
		coords.pop_back();

		enclosed[v.y][v.x] = true;

		for (int j = -1; j <= 1; j++) {
			for (int i = -1; i <= 1; i++) {
				if ((i == 0 || j == 0) && !(i == 0 && j == 0)) {
					size_t y = v.y + j;
					size_t x = v.x + i;
					if (!visited[y][x] && !enclosed[y][x]) {
						coords.push_back(Vector(x, y));
					}
				}
			}
		}
	}

	// output
	for (size_t y = 0; y < field.height; y++) {
		for (size_t x = 0; x < field.width; x++) {
			if (enclosed[y][x]) {
				std::cout << 'I';
			}
			else if (visited[y][x]) {
				std::cout << '.';
			}
			else {
				std::cout << ' ';
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::flush;
	// output end

	// count how many spots are inside the loop.
	size_t count = 0;
	for (size_t y = 0; y < field.height; y++) {
		for (size_t x = 0; x < field.width; x++) {
			if (enclosed[y][x]) {
				count++;
			}
		}
	}

	return count;
}
