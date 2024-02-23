#include <vector>
#include <sstream>
#include <string>
#include <iostream>

#include "solution1.h"

using namespace day6::solution1;

std::vector<size_t> day6::solution1::parse_line(std::string const& line)
{
	std::istringstream iss(line);
	std::string segment;
	std::getline(iss, segment, ':');
	std::vector<size_t> ints;
	size_t i;
	while (iss >> i) {
		ints.push_back(i);
	}
	return ints;
}

std::vector<Race> day6::solution1::parse_lines(std::vector<std::string> const& lines)
{
	auto times = parse_line(lines[0]);
	auto distances = parse_line(lines[1]);
	if (times.size() != distances.size()) {
		throw "number of times and distances must be identical";
	}
	std::vector<Race> races;
	for (size_t i = 0; i < times.size(); i++) {
		races.push_back(Race{
			.time = times[i],
			.distance = distances[i]
		});
	}
	return races;
}

size_t distance_for_charging_time(size_t charging_time, size_t total_time)
{
	_ASSERT(charging_time <= total_time);
	size_t remaining_time = total_time - charging_time;
	size_t charged_speed = charging_time;
	return remaining_time * charged_speed;
}

size_t day6::solution1::Input::solve()
{
	// initial speed: 0 mm/s
	// 1 ms of pressing the button: add 1 mm/s to the speed of the boat
	// final distance = (remaining time - time pressing the button) * accumulated speed

	// goal: find the number of ways with which you can beat the record
	// notes:
	// - we have to find the two initial speeds at which the record is beaten for the first and last time
	// - then we can count the number of ways by simply calculating: last initial speed - first initial speed + 1
	// - we can do this by using a binary search:
	//   - for each initial speed we can determine if we must go to the right or to the left
	//   - then we either find the minimum value or the maximum value

	size_t product = 1;

	for (Race const& race : races) {
		// lower bound (inclusive): the first charging time that beats the record
		size_t la(0), lb(race.time);
		while (la < lb) {
			size_t middle = (la + lb) / 2;
			size_t distance = distance_for_charging_time(middle, race.time);
			if (distance <= race.distance) {
				la = middle + 1;
			}
			if (distance > race.distance) {
				lb = middle;
			}
		}
		_ASSERT(la == lb);
		const size_t lower_bound = la;
		// upper bound (exclusive): the first charging time that doesn't beat the record anymore after the lower bound
		size_t ua(lower_bound), ub(race.time);
		while (ua < ub) {
			size_t middle = (ua + ub) / 2;
			size_t distance = distance_for_charging_time(middle, race.time);
			if (distance <= race.distance) {
				ub = middle;
			}
			if (distance > race.distance) {
				ua = middle + 1;
			}
		}
		_ASSERT(ua == ub);
		const size_t upper_bound = ua;
		_ASSERT(distance_for_charging_time(lower_bound, race.time) > race.distance);
		_ASSERT(distance_for_charging_time(lower_bound - 1, race.time) <= race.distance);
		_ASSERT(distance_for_charging_time(upper_bound, race.time) <= race.distance);
		_ASSERT(distance_for_charging_time(upper_bound - 1, race.time) > race.distance);
		std::cout << race.time << ";" << race.distance << " -> "
			<< lower_bound << ":" << distance_for_charging_time(lower_bound, race.time) << ","
			<< upper_bound << ":" << distance_for_charging_time(upper_bound, race.time) << std::endl;

		const size_t ways_to_win = upper_bound - lower_bound;
		product *= ways_to_win;
	}

	return product;
}
