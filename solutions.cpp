#include <iostream>
#include <vector>
#include <string>

#include "day1_solution1.h"
#include "lib.h"

int main()
{
	std::cout << "day1/solution1/part1/example1: " << day1_solution1::handle_lines(read_lines("./input/day1-example1.txt")) << std::endl;
	std::cout << "day1/solution1/part1/solution: " << day1_solution1::handle_lines(read_lines("./input/day1.txt")) << std::endl;
	std::cout << "day1/solution1/part2/example2: " << day1_solution1::handle_lines(day1_solution1::transformed_written_numbers(read_lines("./input/day1-example2.txt"))) << std::endl;
	std::cout << "day1/solution1/part2/solution: " << day1_solution1::handle_lines(day1_solution1::transformed_written_numbers(read_lines("./input/day1.txt"))) << std::endl;
	std::cout << std::endl;

	//for (std::string const& line : day1_solution1::transformed_written_numbers(read_lines("./input/day1.txt"))) {
	//	std::cout << line << std::endl;
	//}

	return 0;
}
