// std
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <cmath>
#include <set>
#include <sstream>
// local
#include "lib.h"

/*

option 1 is the naive solution, just try all permutations and check if they are right
should work fine with the inputs, since they aren't that large and it's always a fixed number of #'s

option 2:
- make one pass, filtering out certain question marks that are impossible to be a # (we don't need those)
- in that same pass, we might be able to assign certain existing # to part of the input sequence of numbers
  e.g. "?#?#?#?#?#?#?#?" and "1,3,1,6": the first # is definitively the first 1, since the first ? can be removed
  and thus the next # must be corresponding to the first 1 in the sequence
- assign a subsequence of numbers to a subsequence of question marks
- for the rest use dynamic programming to find partial solutions for those pairs of question marks and numbers,
  i.e. reuse solutions

*/

enum class ElementType : char
{
	UNKNOWN = '?',
	OPERATIONAL = '.',
	DAMAGED = '#',
	NONE = '\0'
};

struct Element
{
	ElementType type;
	size_t width;

	Element()
		: type(ElementType::NONE), width(0)
	{}

	Element(ElementType type, size_t width)
		: type(type), width(width)
	{}

	friend bool operator==(Element const&, Element const&) = default;
};

struct Record
{
	std::vector<Element> elements;
	std::vector<size_t> sequence;
};

std::vector<Record> parse_input(std::string const& input)
{
	std::vector<Record> records;
	Record current_record;
	Element current_element;

	std::istringstream iss(input);
	iss >> std::noskipws;

	int n;
	char c;
	char prev = 0;
	bool is_end = false;
	for (; iss; c = 0, n = 0) {
		if (is_end) {
			iss >> n;
			current_record.sequence.push_back(n);
			iss >> c;
			if (c == ',')
				continue;
			if (c == '\n' || !iss) {
				is_end = false;
				records.push_back(current_record);
				current_record = {};
				continue;
			}
			throw "invalid input";
		}
		iss >> c;
		if (c == prev) {
			current_element.width += 1;
			continue;
		}
		if (c == ' ') {
			is_end = true;
		}
		if (current_element.type != ElementType::NONE || is_end) {
			current_record.elements.push_back(current_element);
			current_element = {};
		}
		switch (c) {
		case static_cast<char>(ElementType::DAMAGED):
		case static_cast<char>(ElementType::OPERATIONAL):
		case static_cast<char>(ElementType::UNKNOWN):
			current_element.type = static_cast<ElementType>(c);
			current_element.width = 1;
		}
		prev = c;
	}

	return records;
}

size_t partial_solve(size_t n_unknown, std::vector<size_t> sequence)
{
	// static std::unordered_map<std::pair<>> dp
	return 0;
}

size_t solve()
{
	return 0;
}

// testing
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Tests
{
	using namespace testing;

	std::vector<Record> get_records(std::string const& filename)
	{
		auto input = read_file("./src/day12/input/" + filename + ".txt");
		return parse_input(input);
	}

	size_t get_solution(std::string const& filename, size_t expansion_amount)
	{
		return 0;
	}

	TEST(ParseInput, Parses) {
		auto records = get_records("p1e1");
		ASSERT_EQ(records.size(), 6);
		EXPECT_THAT(records[0].sequence, ElementsAre(1, 1, 3));
		EXPECT_THAT(records[0].elements, ElementsAre(
			Element(ElementType::UNKNOWN, 3),
			Element(ElementType::OPERATIONAL, 1),
			Element(ElementType::DAMAGED, 3)
		));
		EXPECT_THAT(records[5].sequence, ElementsAre(3, 2, 1));
		EXPECT_THAT(records[5].elements, ElementsAre(
			Element(ElementType::UNKNOWN, 1),
			Element(ElementType::DAMAGED, 3),
			Element(ElementType::UNKNOWN, 8)
		));
	}
}
