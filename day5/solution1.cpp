#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <limits>
#include <optional>

#include "solution1.h"
#include "solution2.h"

using namespace day5;
using namespace day5::solution1;

std::vector<int_t> day5::solution1::parse_integers(std::string const& line)
{
	std::istringstream iss(line);
	std::vector<int_t> integers;
	int_t n;
	while (iss >> n) {
		integers.push_back(n);
	}
	return integers;
}

enum State
{
	INITIAL,
	MAP_HEADER,
	MAP_VALUES
};

Almanac day5::solution1::parse_lines(std::vector<std::string> const& lines)
{
	Almanac almanac;
	almanac.seeds = parse_integers(lines[0].substr(lines[0].find_first_of(' ') + 1));

	State state = State::INITIAL;
	Mapping current_mapping;
	std::string discard;

	for (size_t i = 1; i < lines.size(); i++) {
		auto line = lines[i];
		if (line.size() == 0) {
			if (state == State::MAP_VALUES) {
				almanac.mappings[current_mapping.from] = current_mapping;
				current_mapping = {};
			}
			state = State::MAP_HEADER;
			continue;
		}
		std::istringstream iss(line);
		if (state == State::MAP_HEADER) {
			std::getline(iss, current_mapping.from, '-');
			std::getline(iss, discard, '-');
			if (discard != "to")
				throw "expected 'to' token";
			std::getline(iss, current_mapping.to, ' ');
			state = State::MAP_VALUES;
			continue;
		}
		if (state == State::MAP_VALUES) {
			auto integers = parse_integers(line);
			if (integers.size() != 3)
				throw "expected 3 integers";
			current_mapping.ranges.push_back(MappingRange(integers[0], integers[1], integers[2]));
		}
	}

	if (state == State::MAP_VALUES) {
		almanac.mappings[current_mapping.from] = current_mapping;
	}

	// safety: check that none of the mappings overlap
	for (auto const& [_, mapping] : almanac.mappings) {
		for (size_t i = 0; i < mapping.ranges.size(); i++) {
			for (size_t j = i + 1; j < mapping.ranges.size(); j++) {
				auto const& a = mapping.ranges[i];
				auto const& b = mapping.ranges[j];
				solution2::Range x(a.source_range_start, a.length);
				solution2::Range y(b.source_range_start, b.length);
				if (x.has_overlap(y)) {
					throw "overlap among mappings?";
				}
			}
		}
	}

	return almanac;
}

void Mapping::sort_ranges()
{
	// precondition: ranges must be sorted in ascending order by key "Range::source_range_start".
	std::sort(ranges.begin(), ranges.end(),
		[](MappingRange const& a, MappingRange const& b) {
			return a.source_range_start < b.source_range_start;
		});
}

std::optional<std::vector<MappingRange>::const_iterator> Mapping::resolve_range(int_t value) const
{
	auto element = std::upper_bound(ranges.begin(), ranges.end(), value,
		[](int_t number, MappingRange const& range) {
			return number < range.source_range_start + range.length;
		});
	if (element == ranges.end() || value < element->source_range_start) {
		return std::nullopt;
	}
	return element;
}

int_t Mapping::resolve_value(int_t value) const
{
	auto range = resolve_range(value);
	if (range.has_value()) {
		MappingRange r = *range.value();
		return value + (r.destination_range_start - r.source_range_start);
	}
	return value;
}

std::unordered_map<int_t, std::unordered_map<std::string, int_t>> Almanac::resolve_seed_mappings(std::string const& target_key)
{
	if (target_key == initial_mapping_key) {
		throw "target cannot be the initial mapping key";
	}

	// prepare the mapping instances before using them to resolve numbers.
	for (auto& [_, mapping] : mappings) {
		mapping.sort_ranges();
	}

	std::unordered_map<int_t, std::unordered_map<std::string, int_t>> resolved_mappings;

	for (int_t seed : seeds) {
		bool target_reached = false;
		std::string current_key = initial_mapping_key;
		int_t current_value = seed;
		while (mappings.contains(current_key)) {
			auto const& mapping = mappings[current_key];
			std::string const& mapped_key = mapping.to;
			int_t next_value = mapping.resolve_value(current_value);
			resolved_mappings[seed][mapped_key] = next_value;
			if (mapped_key == target_key) {
				target_reached = true;
				break;
			}
			current_key = mapped_key;
			current_value = next_value;
		}
		if (!target_reached) {
			throw "target not found";
		}
	}

	return resolved_mappings;
}

// copied from above, just to be able to get a seed number that gives a location number
int_t Almanac::backtrack_seed_mappings(int_t value, std::string const& target_key, std::string const& start_key)
{
	// sort mappings by destination instead.
	for (auto& [_, mapping] : mappings) {
		std::sort(mapping.ranges.begin(), mapping.ranges.end(),
			[](MappingRange const& a, MappingRange const& b) {
				return a.destination_range_start < b.destination_range_start;
			});
	}

	std::unordered_map<std::string, Mapping> reverse_mappings;
	for (auto const& [_, mapping] : mappings) {
		reverse_mappings[mapping.to] = mapping;
	}

	bool target_reached = false;
	std::string current_key = start_key;
	int_t current_value = value;
	while (reverse_mappings.contains(current_key)) {
		auto const& mapping = reverse_mappings[current_key];
		std::string const& mapped_key = mapping.from;

		int_t next_value(0);
		// resolve code (in reverse)
		auto element = std::upper_bound(mapping.ranges.begin(), mapping.ranges.end(), current_value,
			[](int_t number, MappingRange const& range) {
				return number < range.destination_range_start + range.length;
			});
		if (element == mapping.ranges.end() || current_value < element->destination_range_start) {
			next_value = current_value;
		}
		else {
			MappingRange r = *element;
			next_value = current_value + (r.source_range_start - r.destination_range_start);
		}
		// end

		current_key = mapped_key;
		current_value = next_value;
		if (mapped_key == target_key) {
			target_reached = true;
			break;
		}
	}
	if (!target_reached) {
		throw "target not found";
	}

	return current_value;
}

int_t Input::get_lowest_mapped_value(std::string const& target)
{
	static const int_t maxint = std::numeric_limits<int_t>::max();
	int_t minimum = maxint;
	auto mapped_values = almanac.resolve_seed_mappings(target);
	for (auto const& [seed, values] : mapped_values) {
		auto iterator = values.find(target);
		if (iterator == values.end()) {
			throw "target not found";
		}
		auto value = iterator->second;
		if (value < minimum) {
			minimum = value;
		}
	}
	if (minimum == maxint) {
		throw "no minimum value found";
	}
	return minimum;
}

void Almanac::adapt_seeds_array()
{
	std::vector<int_t> new_seeds;
	for (size_t i = 0; i < seeds.size(); i += 2) {
		for (int_t k = 0; k < seeds[i + 1]; k++) {
			int_t value = seeds[i] + k;
			new_seeds.push_back(value);
		}
	}
	seeds = new_seeds;
}
