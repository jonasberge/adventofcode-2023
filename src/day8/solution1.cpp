#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <array>
#include <unordered_map>

#include "solution1.h"

using namespace day8::solution1;

Map day8::solution1::parse_lines(std::vector<std::string> const& lines)
{
	Map map;
	std::istringstream iss(lines[0]);
	char c;
	while (iss >> c) {
		switch (c) {
		case 'L': map.steps.push_back(0); break;
		case 'R': map.steps.push_back(1); break;
		}
	}
	for (size_t i = 2; i < lines.size(); i++) {
		std::string const& line = lines[i];
		auto from = line.substr(0, 3);
		map.network[from] = { line.substr(7, 3), line.substr(12, 3) };
	}
	return map;
}

size_t day8::solution1::Input::solve()
{
	std::string current = "AAA";
	size_t step_index = 0;
	size_t step_count = 0;

	while (current != "ZZZ") {
		size_t step = map.steps[step_index];
		std::string const& next = map.network[current][step];

		current = next;
		step_count++;

		step_index++;
		if (step_index == map.steps.size()) {
			step_index = 0;
		}
	}

	return step_count;
}
