#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "lib.h"

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
