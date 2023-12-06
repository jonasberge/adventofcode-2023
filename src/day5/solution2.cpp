#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <limits>

#include "solution1.h"
#include "solution2.h"

using namespace day5::solution2;

std::optional<Range> Range::overlap(Range const& other) const
{
	auto a = std::max(begin(), other.begin());
	auto b = std::min(end_inclusive(), other.end_inclusive());
	if (a > b)
		return std::nullopt;
	return Range::from_values(a, b);
}

std::vector<Range> Range::partition_overlaps(Range const& other) const
{
	Range const& self = *this;
	if (self == other) {
		// the ranges are overlapping exactly
		return { self };
	}
	if (!has_overlap(other)) {
		// the ranges are not overlapping
		return { self, other };
	}
	// the ranges are overlapping in such a way, that three new ranges are formed.
	std::pair<int_t, int_t> a = std::minmax(begin(), other.begin());
	std::pair<int_t, int_t> b = std::minmax(end(), other.end());
	std::vector<Range> result;
	if (a.second - a.first > 0) result.push_back(Range(a.first, a.second - a.first));
	if (b.first - a.second > 0) result.push_back(Range(a.second, b.first - a.second));
	if (b.second - b.first > 0) result.push_back(Range(b.first, b.second - b.first));
	return result;
}

int_t Input::minimum_mapped_value(std::string const& target_key)
{
	// - iterate over all mappings
	// - iterate over each mapping's individual mapping ranges
	// - iterate the current set of ranges and split each of them
	//   - add the sub-range that overlaps with the mapping range, mapped to the destination range
	//   - add the sub-range(s) that do not overlap with any mapping range

	// seeds to ranges
	std::vector<Range> range_pool;
	for (size_t i = 0; i < almanac.seeds.size(); i += 2) {
		int_t begin = almanac.seeds[i];
		int_t length = almanac.seeds[i + 1];
		range_pool.push_back(Range(begin, length));
	}

	// iterate mappings
	std::string current_key = initial_mapping_key;
	while (almanac.mappings.contains(current_key)) {
		auto const& mapping = almanac.mappings[current_key];

		std::vector<Range> next; // ranges to handle with next mapping

		std::vector<Range> to_handle; // ranges to handle currently
		to_handle.insert(to_handle.end(), range_pool.begin(), range_pool.end());

		// iterate mapping ranges
		for (MappingRange const& r : mapping.ranges) {
			Range source_range(r.source_range_start, r.length);
			int_t destination_offset = r.offset();

			std::vector<Range> current; // ranges to handle with next mapping range

			// iterate source ranges
			for (Range const& range : to_handle) {

				// handle partitions
				for (Range const& sub_range : range.partition_overlaps(source_range)) {
					if (range.contains(sub_range.begin()) && range.contains(sub_range.end_inclusive())
							&& source_range.contains(sub_range.begin()) && source_range.contains(sub_range.end_inclusive())) {
						// overlaps with source range -> map to destination range.
						// the resulting range will be handled by the next mapping.
						next.push_back(Range(sub_range.begin() + destination_offset, sub_range.length()));
					}
					else if (range.begin() == sub_range.begin() || range.end() == sub_range.end()) {
						// does not overlap with source range -> might overlap with a different mapping range.
						// thus this sub-range must be considered by intersected with the next mapping range.
						current.push_back(sub_range);
					}
				}
			}

			to_handle.swap(current);
		}

		// insert the mapped ranges and the ranges that were not mapped
		// and cannot be mapped to anything, since all mappings were handled.
		range_pool.swap(next);
		range_pool.insert(range_pool.end(), to_handle.begin(), to_handle.end());

		current_key = mapping.to;
		if (current_key == target_key) {
			break;
		}
	}
	if (current_key != target_key) {
		throw "target key not found";
	}

	// find minimum
	int_t minimum = std::numeric_limits<int_t>::max();
	for (Range const& range : range_pool) {
		if (range.begin() < minimum) {
			minimum = range.begin();
		}
	}
	if (minimum == std::numeric_limits<int_t>::max()) {
		throw "minimum not found";
	}

	return minimum;
}

// this implementation is the following approach:
// 1. iterate ranges
// 2. apply appropriate mappings to get sub-ranges/intersections
// 3. handle those new ranges with the next mapping
// which is erroneous and incredibly complex because:
// - the parts outside of the intersection need to be considered too,
//   but only those parts that don't intersect with other mappings
//   (where numbers are mapped to themselves and not to other numbers).
//   this already becomes very complex territory with this approach
// - not doing that (discarding sub-ranges that don't intersect with the current mapping)
//   is erroneous, although (circumstantially) it does give the right solution for the puzzle input.
//
// a simpler approach can be found above.
//
int_t Input::minimum_mapped_value_old(std::string const& target_key)
{
	// seeds to ranges
	std::vector<Range> range_pool;
	for (size_t i = 0; i < almanac.seeds.size(); i += 2) {
		int_t begin = almanac.seeds[i];
		int_t length = almanac.seeds[i + 1];
		range_pool.push_back(Range(begin, length));
	}

	// prepare the ranges for a binary search ("resolve_range" function).
	for (auto& [_, mapping] : almanac.mappings) {
		mapping.sort_ranges();
	}

	// perform mappings to all ranges in the range pool
	// until we have reached the target/destination key.
	std::string current_key = initial_mapping_key;
	while (almanac.mappings.contains(current_key)) {
		// pool that contains ranges which are handled by the next mapping
		std::vector<Range> next_range_pool;
		// pool of ranges which need to still be handled by the current mapping
		std::vector<Range> split_range_pool;
		next_range_pool.reserve(Range::MAX_PARTITION_OVERLAPS * range_pool.size());
		split_range_pool.reserve(Range::MAX_PARTITION_OVERLAPS * range_pool.size());
		auto const& mapping = almanac.mappings[current_key];
		for (Range const& range : range_pool) {
			auto range_iterator = mapping.resolve_range(range.begin());
			if (!range_iterator.has_value()) {
				// none of the values in this range are mapped to other values,
				// i.e. the two ranges do not overlap and no mapping/modification is done.
				next_range_pool.push_back(range);
				continue;
			}

			auto& it = range_iterator.value();
			for (; it != mapping.ranges.end(); it++) {
				MappingRange const& r = *it;
				Range source_range(r.source_range_start, r.length);
				int_t destination_offset = r.offset();

				// end the loop when the source mapping range does not overlap anymore.
				if (!range.has_overlap(source_range))
					break;

				auto partitions = range.partition_overlaps(source_range);
				for (Range const& sub_range : partitions) {
					Range x(0, 1);
					int path_id = -1;
					if (range.contains(sub_range.begin()) && range.contains(sub_range.end_inclusive())
							&& source_range.contains(sub_range.begin()) && source_range.contains(sub_range.end_inclusive())) {
						// this sub-range contains the values that are in
						// BOTH the current range from the range pool AND the mapping's source range.
						// add the destination range to the new range pool, adapted in length.
						if (sub_range.begin() + destination_offset == 0) {
							int x = 0;
						}
						x = Range(sub_range.begin() + destination_offset, sub_range.length());
						path_id = 1;
					}
					else if (range.begin() == sub_range.begin()) {
						// this sub-range contains the values that are in
						// ONLY the current range from the range pool AND NOT in the mapping's source range,
						// since the previous if branch evaluated to false.
						// add the sub-range to the new range pool.
						if (sub_range.begin() == 0) {
							int x = 0;
						}
						x = sub_range;
						path_id = 2;
						continue;
					}
					else if (range.end() == sub_range.end()) {
						// the idea might have been to 
						continue;
					}
					else {
						continue;
					}
					// debugging stuff, not worth cleaning up, as this solution is old anyway
					Range r3(3384979464, 741325372);
					Range r2(979765028, 365717392);
					Range r1(2549393980, 15787022);
					if (r3 == x) {
						int y = 0;
					}
					if (r2 == x) {
						int y = 0;
					}
					if (r1.has_overlap(x)) {
						int y = 0;
					}
					if (x.begin() == 0) {
						int y = 0;
					}
					next_range_pool.push_back(x);
				}
			}
		}
		current_key = mapping.to;
		range_pool = next_range_pool;
		if (current_key == target_key) {
			break;
		}
	}
	if (current_key != target_key) {
		throw "target key not found";
	}

	// find the minimum key in all the ranges.
	int_t minimum = std::numeric_limits<int_t>::max();
	for (Range const& range : range_pool) {
		if (range.begin() == 0) {
			continue;
		}
		if (range.contains(137516820)) {
			int x = 0;
		}
		if (range.begin() < minimum) {
			minimum = range.begin();
		}
	}

	return minimum;
}
