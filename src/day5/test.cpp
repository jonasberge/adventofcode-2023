#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <array>

#include "solution1.h"
#include "solution2.h"
#include "lib.h"

using namespace day5;
using namespace day5::solution1;
using namespace testing;

namespace day5
{
	TEST(Tests, ParseIntegers) {
		EXPECT_THAT(parse_integers("1"), ElementsAre(1));
		EXPECT_THAT(parse_integers("1 2 3"), ElementsAre(1, 2, 3));
		EXPECT_THAT(parse_integers("1936195648 2752993203 133687519"), ElementsAre(1936195648, 2752993203, 133687519));
	}

	class InputTests : public testing::Test
	{
	protected:
		std::vector<std::string> example1_lines = read_lines("./src/day5/input/example1.txt");
		std::vector<std::string> example2_lines = read_lines("./src/day5/input/example2.txt");
		std::vector<std::string> input_lines = read_lines("./src/day5/input/input.txt");

		Almanac example1{
			.seeds = { 79, 14, 55, 13 },
			.mappings = {
				{ "seed", Mapping{ "seed", "soil", { { 50, 98, 2 }, { 52, 50, 48 } } }},
				{ "soil", Mapping{ "soil", "fertilizer", { { 0, 15, 37 }, { 37, 52, 2 }, { 39, 0, 15 } } }},
				{ "fertilizer", Mapping{ "fertilizer", "water", { { 49, 53, 8 }, { 0, 11, 42 }, { 42, 0, 7 }, { 57, 7, 4 } } }},
				{ "water", Mapping{ "water", "light", { { 88, 18, 7 }, { 18, 25, 70 } } }},
				{ "light", Mapping{ "light", "temperature", { { 45, 77, 23 }, { 81, 45, 19 }, { 68, 64, 13 } } }},
				{ "temperature", Mapping{ "temperature", "humidity", { { 0, 69, 1 }, { 1, 0, 69 } } }},
				{ "humidity", Mapping{ "humidity", "location", { { 60, 56, 37 }, { 56, 93, 4 } } }},
			}
		};

		const std::string TARGET_KEY = "location";
	};

	void check_mapping(std::unordered_map<int_t, std::unordered_map<std::string, int_t>> mappings, int_t seed, std::array<int_t, 7> values)
	{
		ASSERT_TRUE(mappings.contains(seed));
		EXPECT_EQ(values[0], mappings[seed]["soil"]);
		EXPECT_EQ(values[1], mappings[seed]["fertilizer"]);
		EXPECT_EQ(values[2], mappings[seed]["water"]);
		EXPECT_EQ(values[3], mappings[seed]["light"]);
		EXPECT_EQ(values[4], mappings[seed]["temperature"]);
		EXPECT_EQ(values[5], mappings[seed]["humidity"]);
		EXPECT_EQ(values[6], mappings[seed]["location"]);
	}

	TEST_F(InputTests, Solution1_Example1) {
		EXPECT_EQ(example1, parse_lines(example1_lines));
		auto resolved_mappings = example1.resolve_seed_mappings(TARGET_KEY);
		check_mapping(resolved_mappings, 79, { 81, 81, 81, 74, 78, 78, 82 });
		check_mapping(resolved_mappings, 14, { 14, 53, 49, 42, 42, 43, 43 });
		check_mapping(resolved_mappings, 55, { 57, 57, 53, 46, 82, 82, 86 });
		check_mapping(resolved_mappings, 13, { 13, 52, 41, 34, 34, 35, 35 });
		Input input{ .almanac = example1 };
		EXPECT_EQ(35, input.get_lowest_mapped_value(TARGET_KEY));
		// part 2
		// just verify that the solution still works with the increased amount of seeds.
		input.almanac.adapt_seeds_array();
		ASSERT_THAT(input.almanac.seeds, ElementsAre(79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67));
		EXPECT_EQ(46, input.get_lowest_mapped_value(TARGET_KEY));
		// backtracking
		EXPECT_EQ(82, input.almanac.backtrack_seed_mappings(46, "seed", "location"));
		EXPECT_EQ(84, input.almanac.backtrack_seed_mappings(46, "soil", "location"));
		EXPECT_EQ(84, input.almanac.backtrack_seed_mappings(46, "fertilizer", "location"));
		EXPECT_EQ(84, input.almanac.backtrack_seed_mappings(46, "water", "location"));
		EXPECT_EQ(77, input.almanac.backtrack_seed_mappings(46, "light", "location"));
		EXPECT_EQ(45, input.almanac.backtrack_seed_mappings(46, "temperature", "location"));
		EXPECT_EQ(46, input.almanac.backtrack_seed_mappings(46, "humidity", "location"));
		for (int_t i = 0; i < 100; i++) {
			int_t target = i;
			int_t original_seed_for_target = example1.backtrack_seed_mappings(target, "seed", "location");
			solution1::Input inp{ .almanac = example1 };
			inp.almanac.seeds = { original_seed_for_target };
			EXPECT_EQ(target, inp.get_lowest_mapped_value("location"));
		}
	}
	TEST_F(InputTests, Solution1_Input) {
		Almanac input_almanac = parse_lines(input_lines);
		Input input{ .almanac = input_almanac };
		EXPECT_EQ(389056265, input.get_lowest_mapped_value(TARGET_KEY));
	}
	TEST_F(InputTests, Solution2_Example1) {
		solution2::Input input{ .almanac = example1 };
		EXPECT_EQ(46, input.minimum_mapped_value_old(TARGET_KEY));
		EXPECT_EQ(46, input.minimum_mapped_value(TARGET_KEY));
	}
	TEST_F(InputTests, Solution2_Example2) {
		solution2::Input input{ .almanac = parse_lines(example2_lines) };
		// NOTE: this test fails with the old solution, which is the reason for the rewrite.
		// EXPECT_EQ(15, input.minimum_mapped_value_old(TARGET_KEY));
		EXPECT_EQ(15, input.minimum_mapped_value(TARGET_KEY));
	}
	TEST_F(InputTests, Solution2_Input) {
		Almanac input_almanac = parse_lines(input_lines);
		solution2::Input input{ .almanac = input_almanac };
		EXPECT_EQ(137516820, input.minimum_mapped_value_old(TARGET_KEY));
		EXPECT_EQ(137516820, input.minimum_mapped_value(TARGET_KEY));
		// Tests for debugging why I'd get 0 as result.
		//int_t target = 0;
		//int_t original_seed_for_target = input.almanac.backtrack_seed_mappings(0, "seed", "location");
		//EXPECT_EQ(3955612581, original_seed_for_target);
		//Almanac input_almanac2 = input_almanac;
		//input_almanac2.seeds = { original_seed_for_target };
		//solution1::Input solution1_input{ .almanac = input_almanac2 };
		//EXPECT_EQ(target, solution1_input.get_lowest_mapped_value("location"));
		//auto seed_mappings = input_almanac2.resolve_seed_mappings("location");
	}

	// TODO: Make a fixture for these tests and call sort_ranges() in the prepare method or so.
	TEST(MappingRange, ResolveRange_ReturnsFirstMatchingRange) {
		// testing this is important for part 2, where we might have multiple mapping ranges
		// which could overlap with a range of seed values.
		// e.g. seed values 10-20 could overlap with ranges 8-10 and 15-16.
		// we would want to get an iterator to 8-10, such that we do not skip any ranges that overlap.
		Mapping m{ "", "", { MappingRange(0, 5, 1), MappingRange(0, 8, 3), MappingRange(0, 15, 2) }};
		m.sort_ranges();
		// 10 would be the lower bound of the range 10-20.
		// we enter the lower bound to find the lowest range that matches first.
		auto resolved_range = m.resolve_range(10);
		ASSERT_TRUE(resolved_range.has_value());
		EXPECT_EQ(MappingRange(0, 8, 3), *resolved_range.value());
	}
	TEST(MappingRange, ResolveRange_ReturnsNothingWithoutMatchingRange) {
		Mapping m{ "", "", { MappingRange(0, 5, 1), MappingRange(0, 8, 3), MappingRange(0, 15, 2) } };
		m.sort_ranges();
		auto resolved_range = m.resolve_range(7);
		ASSERT_FALSE(resolved_range.has_value());
	}

	using solution2::Range;
	static Range Rv(int_t start, int_t end)
	{
		return Range::from_values(start, end);
	}

	TEST(Range, ZeroLengthThrowsException) {
		EXPECT_ANY_THROW(Range(10, 0));
	}
	TEST(Range, FromValues) {
		EXPECT_EQ(Range(10, 2), Rv(10, 11));
		EXPECT_EQ(Range(-10, 2), Rv(-10, -9));
	}
	TEST(Range, PartitionOverlap_NoOverlap) {
		EXPECT_THAT(Rv(1, 10).partition_overlaps(Rv(11, 19)), ElementsAre(Rv(1, 10), Rv(11, 19)));
	}
	TEST(Range, PartitionOverlap_ExactOverlap) {
		EXPECT_THAT(Rv(1, 10).partition_overlaps(Rv(1, 10)), ElementsAre(Rv(1, 10)));
		EXPECT_THAT(Rv(20, 100).partition_overlaps(Rv(20, 100)), ElementsAre(Rv(20, 100)));
	}
	TEST(Range, PartitionOverlap_PartialOverlap) {
		EXPECT_THAT(Rv(1, 10).partition_overlaps(Rv(10, 19)), ElementsAre(Rv(1, 9), Rv(10, 10), Rv(11, 19)));
		EXPECT_THAT(Rv(1, 20).partition_overlaps(Rv(10, 28)), ElementsAre(Rv(1, 9), Rv(10, 20), Rv(21, 28)));
	}
	TEST(Range, Overlap) {
		EXPECT_FALSE(Rv(1, 10).overlap(Rv(11, 20)).has_value());
		EXPECT_EQ(Rv(1, 10).overlap(Rv(1, 10)).value(), Rv(1, 10));
		EXPECT_EQ(Rv(1, 10).overlap(Rv(10, 19)).value(), Rv(10, 10));
	}
}
