#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

#include "solution1.h"

using namespace day9::solution1;

Sequence day9::solution1::parse_line(std::string const& line)
{
	std::istringstream iss(line);
	std::vector<int_t> result;
	for (int_t n; iss >> n;) {
		result.push_back(n);
	}
	return Sequence{
		.values = result
	};
}

std::vector<Sequence> day9::solution1::parse_lines(std::vector<std::string> const& lines)
{
	std::vector<Sequence> result(lines.size());
	std::transform(lines.begin(), lines.end(), result.begin(), ::parse_line);
	return result;
}

const std::vector<size_t>& day9::solution1::PascalTriangle::get_row(size_t index)
{
	if (index >= rows.size()) {
		ensure_rows(index + 1);
	}
	return rows[index];
}

void day9::solution1::PascalTriangle::ensure_rows(size_t amount)
{
	if (rows.size() == 0) {
		rows.push_back({ 1 });
	}
	while (rows.size() < amount) {
		std::vector<size_t> const& last_row = rows[rows.size() - 1];
		std::vector<size_t> next_row;
		next_row.reserve(last_row.size() + 1);
		next_row.push_back(1);
		for (size_t i = 0; i < last_row.size() - 1; i++) {
			next_row.push_back(last_row[i] + last_row[i + 1]);
		}
		next_row.push_back(1);
		rows.push_back(next_row);
	}
}

int_t day9::solution1::Sequence::extrapolate_value(PascalTriangle& pascal_triangle) const
{
	/*	 a	 b	 c	 d	 e	 f	 g
	1:	10	13	16	21	30	45	68
	2:	 3	 3	 5	 9	15	23
	3:	 0	 2	 4	 6	 8
	4:	 2	 2	 2	 2
	5:	 0	 0	 0

	row 1: given
	row 2: calculated from immediate differences
	row 3:
	- a3 = 3-3 = (16-13)-(13-10) = 16-2*13+10
	- a3 = c1-2*b1+a1
	- ...
	- removing "1" suffix since we always use the values from the first row
	- a3 = c-2b+a
	row 4:
	- a4 = b3-a3 = (c2-b2)-(b2-a2) = c2-2*b2+a2 = (d-c)-2*(c-b)+(b-a)
	- a4 = d-c-2c+2b+b-a
	- a4 = d-3c+3b-a
	- b4 = e-3d+3c-b
	row 5:
	- a5 = b4-a4 = (e-3d+3c-b)-(d-3c+3b-a)
	- a5 = e-3d+3c-b-d+3c-3b+a
	- a5 = e-4d+6c-4b+a
	- c5 = g-4f+6e-4d+c
	- multipliers can be generated with a pascal triangle, signs simply alternate
	- <=> g = c5+4f-6e+4d-c
	- we know all values, except c5. if we can guarantee that c5 (or any other row value)
	  must be 0, we can put 0 in its place and easily calculate g.
	- we know that there is some row x on which there are only identical numbers.
	  we also know that that this row is extended by one number identical to the others on that row.
	  that means row x+1 must have a 0. since there can be at most n rows,
	  where n is the number of numbers in the input, row n+1 must always be a 0 (since row n is extended by the same number).
	- we only need an efficient way to generate a row of the pascal triangle.
	- is generating a pascal triangle of size n more efficient than the naive solution for this problem with size n?
	  yes, since the triangle can be reused across invocations. it only has to be generated once for size n.
	*/

	size_t n = values.size();
	// row n has n+1 values, which is what we need
	std::vector<size_t> pt_row = pascal_triangle.get_row(n);
	int_t sign = +1;
	int_t sum = 0;
	// skip the first pascal triangle value
	for (size_t i = 1; i < pt_row.size(); i++) {
		_ASSERT(n >= i);
		_ASSERT(n - i >= 0 && n - i < values.size());
		size_t k = n - i; // reverse index into sequence values
		size_t pt_value = pt_row[i];
		sum += values[k] * pt_value * sign;
		sign = -sign; // flip sign
	}

	return sum;
}

int_t day9::solution1::Sequence::extrapolate_value_backwards(PascalTriangle& pascal_triangle) const
{
	// part 2
	// 0. reverse each sequence:
	/*	 a	 b	 c	 d	 e	 f	 g
	1:	45	30	21	16	13	10	 5
	2:	15	 9	 5	 3	 3	 5
	3:	 6	 4	 2	 0	-2
	4:	 2	 2	 2	 2
	5:	 0	 0	 0

	row 4:
	- a4 = a3-b3 = (a2-b2)-(b2-c2) = a2-2*b2+c2 = (a-b)-2*(b-c)+(c-d)
	- a4 = a-3b+3c-d
	=> 1. the letters are not reversed anymore
	- c5 = c-4d+6e-4f+g
	- <=> g = -c+4d-6e+4f
	=> 2. do not skip the first pascal triangle value
	=> 3. reverse the sign (negative)
	*/

	// values must be reversed (0)
	std::vector<int_t> reversed(values);
	std::reverse(reversed.begin(), reversed.end());

	size_t n = reversed.size();
	// row n has n+1 values, which is what we need
	std::vector<size_t> pt_row = pascal_triangle.get_row(n);
	int_t sign = -1; // start with negative sign (3)
	int_t sum = 0;
	// do not skip the first pascal triangle value (2)
	for (size_t i = 0; i < pt_row.size() - 1; i++) {
		size_t k = i; // index into sequence values, not reversed (1)
		size_t pt_value = pt_row[i];
		sum += reversed[k] * pt_value * sign;
		sign = -sign; // flip sign
	}

	return sum;
}

int_t day9::solution1::Input::solve()
{
	int_t sum = 0;
	PascalTriangle pt;
	for (Sequence const& sequence : sequences) {
		sum += sequence.extrapolate_value(pt);
	}
	return sum;
}

int_t day9::solution1::Input::solve_backwards()
{
	int_t sum = 0;
	PascalTriangle pt;
	for (Sequence const& sequence : sequences) {
		sum += sequence.extrapolate_value_backwards(pt);
	}
	return sum;
}
