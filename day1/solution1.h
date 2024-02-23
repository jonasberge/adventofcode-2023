#pragma once

#include <string>
#include <vector>
#include <regex>

namespace day1
{
	class Solution1
	{
	public:
		int handle_line(std::string const& line);
		int handle_lines(std::vector<std::string> const& lines);
		std::string transform_written_numbers(std::string const& line);
		std::vector<std::string> transformed_written_numbers(std::vector<std::string> const& lines);
	};
}
