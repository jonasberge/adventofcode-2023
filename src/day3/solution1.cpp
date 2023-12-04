#include <cctype>
#include <cmath>
#include <string>
#include <variant>
#include <vector>
#include <utility>
#include <unordered_map>
#include <functional>

#include "lib.h"
#include "solution1.h"

using namespace day3::solution1;

const char SYMBOL_GEAR = '*';
const char SYMBOL_SPACER = '.';
const int GEAR_NEIGHBOUR_COUNT = 2;

Schematic day3::solution1::parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Cell> cells;
	Position current_position = { 0, 0 };

	for (std::string const& line : lines) {
		current_position.x = 0;
		bool is_number = false;
		int current_number = 0;
		Position number_position;
		for (size_t i = 0; i < line.length(); i++, current_position.x++) {
			char c = line[i];
			if (std::isdigit(c)) {
				if (!is_number) {
					number_position = current_position;
				}
				is_number = true;
				current_number *= 10;
				current_number += c - '0';
				continue;
			}
			if (is_number) {
				cells.push_back(Cell(number_position, current_number));
				is_number = false;
				current_number = 0;
			}
			if (c == SYMBOL_SPACER) {
				continue;
			}
			cells.push_back(Cell(current_position, c));
		}
		if (is_number) {
			cells.push_back(Cell(number_position, current_number));
		}
		current_position.y += 1;
	}

	return Schematic{
		.width = static_cast<int>(lines[0].size()),
		.height = static_cast<int>(lines.size()),
		.cells = cells
	};
}

int Cell::get_width() const
{
	if (std::holds_alternative<char>(value)) {
		return 1;
	}
	if (int const* v = std::get_if<int>(&value)) {
		int value = *v;
		int log = value > 0 ? static_cast<int>(std::floor(std::log10(value))) : 0;
		return log + 1;
	}
	unreachable();
}

std::vector<Cell> Schematic::get_part_numbers() const
{
	std::vector<Cell> part_numbers;
	std::vector<bool> map(width * height);

	for (Cell const& cell : cells) {
		if (std::holds_alternative<char>(cell.value)) {
			for (int y = cell.position.y - 1; y <= cell.position.y + 1; y++) {
				for (int x = cell.position.x - 1; x <= cell.position.x + 1; x++) {
					int index = Position(x, y).get_flat_index(width);
					if (index >= 0 && index < map.size()) {
						map[index] = true;
					}
				}
			}
		}
	}

	for (Cell const& cell : cells) {
		if (int const* v = std::get_if<int>(&cell.value)) {
			int value = *v;
			int cell_width = cell.get_width();
			for (int x = cell.position.x; x < cell.position.x + cell_width; x++) {
				Position position(x, cell.position.y);
				int index = position.get_flat_index(width);
				if (map[index]) {
					part_numbers.push_back(cell);
					break;
				}
			}
		}
	}

	return part_numbers;
}

std::vector<Gear> Schematic::get_gears() const
{
	auto hash = [this](Position const& p) {
		return std::hash<int>()(p.get_flat_index(width));
	};
	std::unordered_map<Position, std::vector<Cell>, decltype(hash)> gears(8, hash);
	const int size = width * height;

	// initialize the map with the positions of all gear symbols (*).
	for (Cell const& cell : cells) {
		if (char const* c = std::get_if<char>(&cell.value); c && *c == SYMBOL_GEAR) {
			gears[cell.position] = {};
		}
	}

	// assign each part number to its adjacent gear symbol(s).
	for (Cell const& cell : get_part_numbers()) {
		int const* v = std::get_if<int>(&cell.value);
		if (!v)
			continue;

		int value = *v;
		int cell_width = cell.get_width();
		// iterate neighbours around each of the value's digits.
		for (int y = cell.position.y - 1; y <= cell.position.y + 1; y++) {
			for (int x = cell.position.x - 1; x <= cell.position.x + cell_width; x++) {
				if (x < 0 || x >= width || y < 0 || y >= height)
					continue;
				Position position(x, y);
				if (gears.find(position) != gears.end()) {
					gears[position].push_back(cell);
				}
			}
		}
	}

	// filter out those gear symbols with
	// the exact required number of part number neighbours.
	std::vector<Gear> result;
	result.reserve(gears.size());
	for (auto const& [position, neighbours] : gears) {
		if (neighbours.size() == GEAR_NEIGHBOUR_COUNT) {
			result.push_back({
				.position = position,
				.part_numbers = {
					std::get<int>(neighbours[0].value),
					std::get<int>(neighbours[1].value),
				}
			});
		}
	}

	return result;
}

int Input::sum_of_part_numbers() const
{
	int sum = 0;
	std::vector<Cell> part_numbers = schematic.get_part_numbers();
	for (Cell const& cell : part_numbers) {
		sum += std::get<int>(cell.value);
	}
	return sum;
}

int Input::sum_of_gear_ratios() const
{
	int sum = 0;
	std::vector<Gear> gears = schematic.get_gears();
	for (Gear const& gear : gears) {
		sum += gear.gear_ratio();
	}
	return sum;
}
