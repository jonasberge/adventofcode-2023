#pragma once

#include <string>
#include <vector>
#include <utility>
#include <optional>
#include <array>
#include <unordered_set>
#include <functional>

namespace day10::solution1
{
	enum class Direction
	{
		North = 5,
		East = 4,
		South = 3,
		West = 2,
		Unknown = 1,
		None = 0
	};

	enum class Turn
	{
		LeftTurn = 2,
		RightTurn = 1,
		None = 0
	};

	using PipeDirections = std::pair<Direction, Direction>;

	static const PipeDirections PIPE_SIDES_NONE = { Direction::None, Direction::None };
	static const PipeDirections PIPE_SIDES_START = { Direction::Unknown, Direction::Unknown };

	struct Vector
	{
		size_t x;
		size_t y;

		Vector(size_t x, size_t y) : x(x), y(y) {}

		Vector plus(Vector other) const
		{
			return Vector(x + other.x, y + other.y);
		}

		static Vector for_direction(Direction direction)
		{
			switch (direction) {
			case Direction::North: return Vector(0, -1);
			case Direction::East: return Vector(1, 0);
			case Direction::South: return Vector(0, 1);
			case Direction::West: return Vector(-1, 0);
			default: return Vector(0, 0);
			}
		}
	};

	struct Pipe
	{
		PipeDirections sides = PIPE_SIDES_NONE;

		Pipe() {}
		Pipe(std::pair<Direction, Direction> sides) {
			this->sides = sides;
		}

		inline bool contains(Direction direction) const
		{
			return sides.first == direction || sides.second == direction;
		}

		// 1. get opposite end coordinates for given coordinates
		std::pair<Vector, Vector> opposites(Vector position)
		{
			
		}

	private:
		Vector opposite(Vector position, Direction direction)
		{

		}
	};

	struct Field
	{
		size_t start_x;
		size_t start_y;
		size_t width;
		size_t height;
		std::vector<std::vector<std::vector<Direction>>> directions_grid;

		Field(size_t height, size_t width)
			: start_x(0), start_y(0), width(width), height(height)
		{
			directions_grid.insert(directions_grid.begin(), height, std::vector<std::vector<Direction>>(width));
		}

		void iterate_both_directions(std::function<void(const Vector&, const Vector&)> callback);
		// callback: first vector is the the direction we're going, second vector is the previous direction
		// that way we can determine a turn
		void iterate_single_direction(size_t index, std::function<void(const Vector&, Direction, Turn)> callback);
	};

	struct Input
	{
		// parsed input data
		std::vector<std::vector<Pipe>> grid;

		// functions that give solutions or solve parts
		Field parse_grid();

		size_t solve();
	};

	// input parsing functions
	std::vector<Pipe> parse_line(std::string const& line);
	std::vector<std::vector<Pipe>> parse_lines(std::vector<std::string> const& lines);
}
