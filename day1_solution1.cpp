#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

#include "day1_solution1.h"

// C++ features used:
// handle_line:
// - istringstream
// handle_lines:
// - vector
// transform_written_numbers:
// - regex

namespace day1_solution1 {
	int handle_line(std::string const& line)
	{
		std::istringstream iss(line);
		char letter;
		int number(-1);
		int digits_count(0);
		while ((iss >> letter) && !iss.eof()) {
			if (std::isdigit(letter)) {
				int digit = letter - '0';
				if (number == -1) {
					number = 10 * digit;
				}
				else {
					// replace the last digit of the current number.
					number = number / 10 * 10 + digit;
				}
				digits_count++;
			}
		}
		if (number < 0) {
			return 0;
		}
		// set the last digit to the first,
		// if there was no second digit.
		if (digits_count == 1) {
			number += number / 10;
		}
		return number;
	}

	int handle_lines(std::vector<std::string> const& lines)
	{
		int sum = 0;
		for (std::string const& line : lines) {
			sum += handle_line(line);
		}
		return sum;
	}

	const std::regex number_regex("(one)|(two)|(three)|(four)|(five)|(six)|(seven)|(eight)|(nine)");

	// [part 2] transforms written numbers to digits
	std::string transform_written_numbers(std::string const& line)
	{
		// determines how far we step after matching one of the number words.
		static const std::size_t STEP_SIZE = 1;

		std::ostringstream oss;
		std::smatch matches;
		std::string::const_iterator search_start(line.cbegin());
		std::size_t previous_match_length(0);
		while (std::regex_search(search_start, line.cend(), matches, number_regex)) {
			std::size_t n_overlap = 0;
			if (search_start != line.cbegin()) {
				// determine how much of the previous match is in this match's prefix
				// and remove it. this is necessary, since the step size is constant
				// and not always as large as the match itself.
				// so parts of a match can be in the prefix of the next match,
				// which need to be removed here, with an appropriate substr() call.
				n_overlap = matches.prefix().second - search_start;
				n_overlap = std::min(n_overlap, previous_match_length - STEP_SIZE);
				n_overlap = std::max(n_overlap, std::size_t(0));
			}
			oss << matches.prefix().str().substr(n_overlap);
			for (auto index = 1; index < matches.size(); ++index) {
				if (matches[index].matched) {
					oss << index;
					previous_match_length = matches[index].length();
					break;
				}
			}
			// start the next search one letter after our match,
			// so we can match overlapping words, like "oneight" as "18".
			// this is a requirement, even though it is not explicitly stated
			// in the problem description
			search_start = matches[0].first + STEP_SIZE;
			//search_start = matches.suffix().first;
		}
		if (search_start == line.cbegin() && matches.empty()) {
			return line;
		}
		oss << matches.suffix().str();
		return oss.str();
	}

	std::vector<std::string> transformed_written_numbers(std::vector<std::string> const& lines)
	{
		std::vector<std::string> result;
		result.reserve(lines.size());
		for (std::string const& line : lines) {
			result.push_back(transform_written_numbers(line));
		}
		return result;
	}
}
