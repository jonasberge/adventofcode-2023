#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <optional>

namespace day5::solution1
{
	typedef long long int_t;

	static const std::string initial_mapping_key = "seed";

	struct MappingRange
	{
		int_t destination_range_start;
		int_t source_range_start;
		int_t length;

		MappingRange() : destination_range_start(0), source_range_start(0), length(0) {}
		MappingRange(int_t destination_range_start, int_t source_range_start, int_t length)
			: destination_range_start(destination_range_start), source_range_start(source_range_start), length(length) {}

		// the offset from source values to destination values.
		inline int_t offset() const { return destination_range_start - source_range_start; }

		friend bool operator==(MappingRange const&, MappingRange const&) = default;
	};

	struct Mapping
	{
		std::string from;
		std::string to;
		std::vector<MappingRange> ranges;

		// sorts the mapping ranges in ascending order by "source_range_start",
		// so that a binary search can be used to find a mapping that corresponds to a specific value.
		void sort_ranges();

		std::optional<std::vector<MappingRange>::const_iterator> resolve_range(int_t value) const;
		int_t resolve_value(int_t value) const;

		friend bool operator==(Mapping const&, Mapping const&) = default;
	};

	struct Almanac
	{
		std::vector<int_t> seeds;
		std::unordered_map<std::string, Mapping> mappings;

		std::unordered_map<int_t, std::unordered_map<std::string, int_t>> resolve_seed_mappings(std::string const& target);
		// adapts the seeds array for part 2, where the range of seeds is a mapping of pairs.
		// this is of course a very naive approach, which uses way too much storage space.
		void adapt_seeds_array();

		int_t backtrack_seed_mappings(int_t value, std::string const& target_key, std::string const& start_key);

		friend bool operator==(Almanac const&, Almanac const&) = default;
	};

	struct Input
	{
		// parsed input data
		Almanac almanac;

		// functions that give solutions or solve parts
		int_t get_lowest_mapped_value(std::string const& target);
	};

	// input parsing functions
	std::vector<int_t> parse_integers(std::string const& line);
	Almanac parse_lines(std::vector<std::string> const& lines);
}
