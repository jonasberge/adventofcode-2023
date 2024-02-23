#include <iostream>

#include "solution1.h"
#include "lib.h"

using namespace day1;

int main()
{
	Solution1 s1;

	auto d1 = read_lines("./day01/input/input.txt");
	auto e1 = read_lines("./day01/input/example1.txt");
	auto e2 = read_lines("./day01/input/example2.txt");

	std::cout << "part1/example1: " << s1.handle_lines(e1) << std::endl;
	std::cout << "part1: " << s1.handle_lines(d1) << std::endl;
	std::cout << std::endl;
	std::cout << "part2/example2: " << s1.handle_lines(s1.transformed_written_numbers(e2)) << std::endl;
	std::cout << "part2: " << s1.handle_lines(s1.transformed_written_numbers(d1)) << std::endl;
	std::cout << std::endl;
	return 0;
}
