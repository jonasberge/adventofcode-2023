#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "solution1.h"

// solution 1 used a naive approach, where we would transform each seed individually.
// that was acceptable then, where we only determined the mapped value of a few seeds,
// but now we would have to determine the value of millions of seeds.
// for that a different, more optimized solution is necessary.
//
// the idea now is to pay attention to the fact that both the seeds and the mappings
// use "ranges" of numbers. when we have a range of seed values and a range of values
// that are mapped to a different range of values, then we effectively partition the
// seed range into 2 or 3 ranges:
// - the part(s) of the original seed range which is left untouched (1 or 2 ranges)
// - the part of the original seed range which is mapped to a different range with one of the mappings (0 or 1 range)
// complexity-wise this only handles a few ranges at the start and then
// splits them up into a few more ranges with possibly different values.
//
// space complexity:
// - a range only uses a few bytes to store the start and its length (or end value), which are integers
// - a range can only be split into at most 3 ranges per mapping step (e.g. seeds to soil)
// - since there are only 7 mapping steps, that means there will be at most 3^7 = 2187 ranges in storage at the end
//
// algorithm:
// NOTE: slightly changed this when I found out it wouldn't give the right answer in all cases
// - create the seed ranges and add them to the range pool
// - for each mapping step:
//   - create a new empty range pool
//   - partition each range from the existing range pool with the current map (of the mapping step) like so:
//     1. create a sub-range which contains all seed values which are also a start value of the mapping
//     2. create 0 to 2 more uninterrupted ranges which contain the remaining seed values that are not mapped
//     - each seed value should be in exactly one of these ranges
//	   - transform the first sub-range with the mapping table to a different range of values
//   - add the created sub-ranges to the new range pool
//   - set the range pool to the new range pool and continue to the next mapping step
// - after all mapping steps, we should end up with a set of all different ranges, to which seeds can be mapped
// - optionally, we can store from which seed range each sub-range originated,
//   if we want to backtrack a result value's original seed value
//
// time complexity:
// - one issue could be, that we have to match each range from the current range pool (up to 2187)
//   with each range of the current mappings (about 10-40), which would be n~10^3-10^4 in the worst case,
//   which is very managable
// - since it's only ever at most one range of the mapping we use to transform a specific seed range
//   we can optimize finding it by e.g. using a binary search algorithm, which would cut down the time to about n~10^3
//
namespace day5::solution2
{
	using namespace day5;

	using solution1::int_t;
	using solution1::Almanac;
	using solution1::Mapping;
	using solution1::MappingRange;

	using solution1::initial_mapping_key;

	// represents a range of values.
	class Range
	{
		int_t m_start;
		size_t m_length;

	public:
		Range(int_t start, size_t length) : m_start(start), m_length(length)
		{
			if (length == 0) {
				throw "the length of a range may not be zero";
			}
		}

		static inline Range from_values(int_t start, int_t end)
		{
			return Range(start, end - start + 1);
		}

		inline int_t begin() const { return m_start; }
		inline int_t end() const { return m_start + m_length; }
		inline int_t end_inclusive() const { return end() - 1; }
		inline int_t length() const { return m_length; }

		inline bool has_overlap(Range const& other) const
		{
			// TODO: negate properly
			return !(end_inclusive() < other.begin() || other.end_inclusive() < begin());
		}

		inline bool contains(int_t value) const
		{
			return value >= begin() && value <= end_inclusive();
		}

		// creates a copy of this range that is shifted by a given offset (positive or negative).
		inline Range shifted(int_t offset) const
		{
			return Range(m_start + offset, m_length + offset);
		}

		// returns the range of values that are present in both ranges.
		std::optional<Range> overlap(Range const& other) const;

		// overlaps this range with another, creating 1 to 3 partitions
		// which contain the following elements:
		// 1. elements that are only in this range (A)
		// 2. elements that are only in the other range (B)
		// 3. elements that are in both ranges A and B
		// in the following cases the given amount of elements is returned:
		// - no overlap between the two ranges: 2 elements
		// - the ranges overlap partly, but not completely: 3 elements
		// - the ranges overlap exactly, i.e. they are identical: 1 element
		std::vector<Range> partition_overlaps(Range const& other) const;

		// maximum amount of elements that can be returned from partition_overlaps().
		static const size_t MAX_PARTITION_OVERLAPS = 3;

		friend bool operator==(Range const&, Range const&) = default;
	};

	struct Input
	{
		// parsed input data
		Almanac almanac;
		
		// functions that give solutions or solve parts
		int_t minimum_mapped_value(std::string const& target);
		int_t minimum_mapped_value_old(std::string const& target);
	};
}
