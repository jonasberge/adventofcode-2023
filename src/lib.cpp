#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

#include "lib.h"

std::string read_file(std::string const& filename)
{
	// std::cout << std::filesystem::current_path() << std::endl;
	std::ifstream ifs(filename);
	std::stringstream buffer;
	buffer << ifs.rdbuf();
	return buffer.str();
}

std::vector<std::string> read_lines(std::string const& filename)
{
	std::ifstream ifs(filename);
	std::vector<std::string> lines;
	lines.reserve(16);
	std::string line;
	while (std::getline(ifs, line)) {
		lines.push_back(line);
	}
	return lines;
}
