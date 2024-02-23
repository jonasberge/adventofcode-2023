// std
#include <vector>
#include <string>
#include <utility>
#include <tuple>
#include <algorithm>
#include <cmath>
#include <set>
#include <sstream>
#include <unordered_map>
#include <cstdint>
// local
#include "lib.h"

/*

option 1: naive solution, try all possibilities and check if they work with the pattern
option 2: dynamic programming, while trying all possibilities, reusing old results

*/

namespace helper {
	// https://stackoverflow.com/a/27216842
	class uint32_vector_hasher {
	public:
		std::size_t operator()(std::vector<uint32_t> const& vec) const {
			std::size_t seed = vec.size();
			for (auto& i : vec) {
				seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			}
			return seed;
		}
	};

	// https://stackoverflow.com/a/2595226
	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	using tuple_type = std::tuple<std::string, size_t, std::vector<uint32_t>, size_t>;

	struct tuple_hash {
		std::size_t operator () (const tuple_type& p) const {
			auto hash = std::hash<std::string>{}(std::get<0>(p));
			auto h2 = std::hash<size_t>{}(std::get<1>(p));
			auto h3 = uint32_vector_hasher{}(std::get<2>(p));
			auto h4 = std::hash<size_t>{}(std::get<3>(p));
			hash_combine(hash, h2);
			hash_combine(hash, h3);
			hash_combine(hash, h4);
			return hash;
		}
	};
}

struct Record
{
	static const size_t UNFOLD_AMOUNT = 5;

	std::string pattern;
	std::vector<uint32_t> sequence;

	Record() {}

	Record(std::string const& pattern, std::vector<uint32_t> const& sequence)
		: pattern(pattern), sequence(sequence)
	{}

	Record unfolded() const
	{
		Record result;
		std::ostringstream oss;
		oss << pattern;
		for (size_t i = 1; i < UNFOLD_AMOUNT; i++) {
			oss << '?' << pattern;
		}
		result.pattern = oss.str();
		result.sequence.reserve(UNFOLD_AMOUNT * sequence.size());
		for (size_t i = 0; i < UNFOLD_AMOUNT; i++) {
			result.sequence.insert(result.sequence.end(), sequence.begin(), sequence.end());
		}
		return result;
	}
};

std::vector<Record> unfolded(std::vector<Record> const& records)
{
	std::vector<Record> result;
	result.reserve(records.size());
	for (auto const& record : records) {
		result.push_back(record.unfolded());
	}
	return result;
}

Record parse_line(std::string const& line)
{
	std::istringstream iss(line);
	Record record;
	iss >> record.pattern;
	uint32_t n;
	while (iss >> n) {
		_ASSERT(n > 0);
		record.sequence.push_back(n);
		if (iss.eof()) break;
		char c = iss.get();
		_ASSERT(c == ',');
	}
	return record;
}

std::vector<Record> parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Record> result;
	result.reserve(lines.size());
	for (auto const& line : lines) {
		result.push_back(parse_line(line));
	}
	return result;
}

size_t solve(std::string const& pattern, size_t index, std::vector<uint32_t> const& sequence, size_t offset)
{
	_ASSERT(offset < sequence.size());

	static std::unordered_map<helper::tuple_type, size_t, helper::tuple_hash> dp;
	auto args = std::make_tuple(pattern, index, sequence, offset);
	if (dp.contains(args)) {
		return dp[args];
	}

	uint32_t required = sequence[offset];
	bool has_more = offset + 1 < sequence.size();

	size_t sum = 0;

	for (size_t i = index; i < pattern.size(); i++) {
		char c = pattern[i];
		_ASSERT(c == '?' || c == '.' || c == '#');

		// skip operational ones.
		if (c == '.') continue;

		// end this loop after we found the first '#' symbol,
		// since that symbol must be included in the first required sequence.
		// also end if this is the last element in the required sequence.
		bool end_after = c == '#' || !has_more;

		if (has_more) {
			bool fits = false;
			for (size_t j = i; j < i + required; j++) {
				if (pattern[j] != '#' && pattern[j] != '?')
					break;
				if (j + 1 == i + required)
					fits = true;
			}
			// the char after this fit may not be a '#'
			if (fits && i + required < pattern.size() && pattern[i + required] == '#')
				fits = false;
			// add 1 since the next char must be '.'
			size_t start_index = i + required + 1;
			if (fits && start_index < pattern.size()) {
				sum += solve(pattern, start_index, sequence, offset + 1);
			}
		}
		else {
			size_t fits_count = 0;
			for (size_t offset = 0; i + required + offset <= pattern.size(); offset++) {
				bool fits = false;
				for (size_t j = i + offset; j < i + required + offset; j++) {
					if (pattern[j] != '#' && pattern[j] != '?')
						break;
					if (j + 1 == i + required + offset)
						fits = true;
				}
				// check the remaining chars, since "has_more" is false
				// there may not be any trailing '#'.
				for (size_t j = i + required + offset; j < pattern.size(); j++) {
					if (pattern[j] == '#') {
						fits = false;
					}
				}
				if (fits) {
					fits_count++;
				}
				if (pattern[i + offset] == '#') {
					// this one must be included and may not be left behind,
					// thus stop here.
					break;
				}
			}
			sum += fits_count;
		}

		if (end_after) break;
	}

	dp[args] = sum;

	return sum;
}

size_t solve(std::string const& pattern, std::vector<uint32_t> const& sequence)
{
	return solve(pattern, 0, sequence, 0);
}

size_t solve(Record const& record)
{
	return solve(record.pattern, record.sequence);
}

size_t solve(std::vector<Record> const& records)
{
	size_t sum = 0;
	for (auto const& record : records) {
		sum += solve(record);
	}
	return sum;
}

// testing
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Tests
{
	using namespace testing;

	TEST(ParseLine, Parses) {
		auto r = parse_line("?###???????? 3,2,1");
		EXPECT_EQ(r.pattern, "?###????????");
		EXPECT_THAT(r.sequence, ElementsAre(3, 2, 1));
	}

	TEST(Solve, Tests) {
		EXPECT_EQ(8, solve("????????", { 1 }));
		EXPECT_EQ(7, solve("????????", { 2 }));
		EXPECT_EQ(1, solve("???", { 1, 1 }));
		EXPECT_EQ(3, solve("????", { 1, 1 }));
		EXPECT_EQ(2, solve("#???", { 1, 1 }));
	}

	TEST(Solve, Example1) {
		auto lines = read_lines("./src/day12/input/p1e1.txt");
		EXPECT_EQ(1, solve(parse_line(lines[0])));
		EXPECT_EQ(4, solve(parse_line(lines[1])));
		EXPECT_EQ(1, solve(parse_line(lines[2])));
		EXPECT_EQ(1, solve(parse_line(lines[3])));
		EXPECT_EQ(4, solve(parse_line(lines[4])));
		EXPECT_EQ(10, solve(parse_line(lines[5])));
		EXPECT_EQ(21, solve(parse_lines(lines)));
	}

	TEST(Solve, SkipsTrailingDamagedSprings) {
		// The trailing '#' should not be counted with certain arrangements.
		EXPECT_EQ(2, solve(parse_line("?#????.????..???#. 6,3,2")));
	}

	TEST(Solve, LastSequenceItemMayNotExceedLastDamagedSpring) {
		// There also shouldn't be a leading '#' left behind.
		EXPECT_EQ(1, solve(parse_line("#????? 1")));
	}

	TEST(Solve, Part1) {
		// attempt 1: 10985, too high => fixed with SkipsTrailingDamagedSprings
		// attempt 2: 9232, too high => fixed with LastSequenceItemMayNotExceedLastDamagedSpring
		// attempt 3: 7857

		auto lines = read_lines("./src/day12/input/in.txt");
		EXPECT_EQ(7857, solve(parse_lines(lines)));
	}

	TEST(Record, Unfolded) {
		auto const& r = Record(".#", { 1 }).unfolded();
		EXPECT_EQ(r.pattern, ".#?.#?.#?.#?.#");
		EXPECT_THAT(r.sequence, ElementsAre(1, 1, 1, 1, 1));
	}

	TEST(Solve, Part2Example1) {
		auto lines = read_lines("./src/day12/input/p1e1.txt");
		EXPECT_EQ(1, solve(parse_line(lines[0]).unfolded()));
		EXPECT_EQ(16384, solve(parse_line(lines[1]).unfolded()));
		EXPECT_EQ(1, solve(parse_line(lines[2]).unfolded()));
		EXPECT_EQ(16, solve(parse_line(lines[3]).unfolded()));
		EXPECT_EQ(2500, solve(parse_line(lines[4]).unfolded()));
		EXPECT_EQ(506250, solve(parse_line(lines[5]).unfolded()));
		EXPECT_EQ(525152, solve(unfolded(parse_lines(lines))));
	}

	TEST(Solve, Part2) {
		// attempt 1: <no result>, no optimization
		// attempt 2: 28606137449920, dynamic programming,
		// especially effective since there are 5 identical copies of everything

		auto lines = read_lines("./src/day12/input/in.txt");
		EXPECT_EQ(28606137449920, solve(unfolded(parse_lines(lines))));
	}
}
