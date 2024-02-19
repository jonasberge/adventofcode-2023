// std
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <cmath>
// testing
#include <gtest/gtest.h>
#include <gmock/gmock.h>
// local
#include "lib.h"


using Point = std::pair<size_t, size_t>;

struct Image
{
	// x,y pairs of galaxy locations
	std::vector<Point> galaxies;

	// indices of rows and columns that do not contain a galaxy
	std::vector<size_t> empty_rows;
	std::vector<size_t> empty_columns;

	bool empty_rows_cols_invalid = false;

	// expands the galaxy, by "duplicating" empty rows and columns.
	void expand();
};

Image parse_input(const std::string& input)
{
	if (input.size() == 0)
		throw "invalid input";

	Image result_image;

	size_t n_cols = input.find_first_of('\n');
	size_t n_rows = (input.size() + 1) / (n_cols + 1);
	std::vector<bool> empty_cols(n_cols, true);
	std::vector<bool> empty_rows(n_rows, true);

	for (size_t i = 0; i < input.size(); i++) {
		size_t y = i / (n_cols + 1);
		size_t x = (i - y) % n_cols;
		_ASSERT(x < empty_cols.size());
		_ASSERT(y < empty_rows.size());
		char c = input[i];
		switch (c) {
		case '#':
			result_image.galaxies.push_back(std::make_pair(x, y));
			empty_cols[x] = false;
			empty_rows[y] = false;
			break;
		case '.': break;
		case '\n': break;
		default: throw "invalid input";
		}
	}

	for (size_t x = 0; x < n_cols; x++)
		if (empty_cols[x])
			result_image.empty_columns.push_back(x);
	for (size_t y = 0; y < n_rows; y++)
		if (empty_rows[y])
			result_image.empty_rows.push_back(y);

	return result_image;
}

void Image::expand()
{
	if (empty_rows_cols_invalid)
		throw "not implemented: cannot expand more than once";

	std::vector<Point> result;

	// cols
	std::sort(galaxies.begin(), galaxies.end(),
		[](Point const& a, Point const& b) {
			return a.first < b.first;
		});
	auto it = empty_columns.begin();
	size_t x_offset = 0;
	for (Point const& galaxy : galaxies) {
		size_t x = galaxy.first;
		size_t y = galaxy.second;
		while (it != empty_columns.end() && *it < x) {
			x_offset++;
			it++;
		}
		result.push_back(std::make_pair(x + x_offset, y));
	}

	galaxies.swap(result);
	result.clear();
	
	// rows
	std::sort(galaxies.begin(), galaxies.end(),
		[](Point const& a, Point const& b) {
			return a.second < b.second;
		});
	it = empty_rows.begin();
	size_t y_offset = 0;
	for (Point const& galaxy : galaxies) {
		size_t x = galaxy.first;
		size_t y = galaxy.second;
		while (it != empty_rows.end() && *it < y) {
			y_offset++;
			it++;
		}
		result.push_back(std::make_pair(x, y + y_offset));
	}

	galaxies.swap(result);

	empty_rows_cols_invalid = true;
}

template<typename Container, typename Action>
void combinations(Container&& container, Action&& callback)
{
	auto first1 = std::begin(container);
	auto last = std::end(container);
	if (first1 == last)
		return;
	auto first2 = first1 + 1;
	while (first1 != last && first2 != last) {
		callback(*first1, *first2);
		if (++first2 == last) {
			first1++;
			if (first1 != last) {
				first2 = first1 + 1;
			}
		}
	}
}

// calculates the taxicab distance between two points
size_t taxicab_distance(Point const& p1, Point const& p2)
{
	return std::abs((long long)p1.first - (long long)p2.first)
		+ std::abs((long long)p1.second - (long long)p2.second);
}

// calculates the sum of taxicab distances between all galaxies
size_t solve(Image& image)
{
	image.expand();

	size_t sum = 0;
	combinations(image.galaxies,
		[&](Point const& p1, Point const& p2) -> void {
			sum += taxicab_distance(p1, p2);
		});

	return sum;
}

namespace Tests
{
	using namespace testing;

	Image get_image(std::string const& filename)
	{
		auto input = read_file("./src/day11/input/" + filename + ".txt");
		auto image = parse_input(input);
		return image;
	}

	size_t get_solution(std::string const& filename)
	{
		Image image = get_image(filename);
		return solve(image);
	}

	TEST(ParseInput, Parses) {
		auto image = get_image("t1");
		ASSERT_THAT(image.galaxies, UnorderedElementsAre(
			Pair(3, 0),
			Pair(7, 1),
			Pair(0, 3)
		));
		ASSERT_THAT(image.empty_rows, UnorderedElementsAre(2));
		ASSERT_THAT(image.empty_columns, UnorderedElementsAre(1, 2, 4, 5, 6, 8, 9));
	}

	TEST(ExpandGalaxy, Expands) {
		auto image = get_image("t1");
		image.expand();
		ASSERT_THAT(image.galaxies, UnorderedElementsAre(
			Pair(5, 0),
			Pair(12, 1),
			Pair(0, 4)
		));
	}

	TEST(Combinations, ThreeElements) {
		std::vector<size_t> elements = { 1, 2, 3 };
		std::vector<std::pair<size_t, size_t>> result;
		combinations(elements, [&](size_t a, size_t b) {
			result.push_back(std::make_pair(a, b));
		});
		ASSERT_THAT(result, UnorderedElementsAre(
			Pair(1, 2),
			Pair(1, 3),
			Pair(2, 3)
		));
	}

	TEST(Solve, Part1) {
		ASSERT_EQ(32, get_solution("t1"));
		ASSERT_EQ(374, get_solution("p1e1"));
		ASSERT_EQ(10276166, get_solution("in"));
	}
}
