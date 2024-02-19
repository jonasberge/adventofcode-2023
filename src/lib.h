#pragma once

#include <vector>
#include <string>

std::string read_file(std::string const& filename);
std::vector<std::string> read_lines(std::string const& filename);

// https://en.cppreference.com/w/cpp/utility/unreachable
[[noreturn]] inline void unreachable()
{
	// Uses compiler specific extensions if possible.
	// Even if no extension is used, undefined behavior is still raised by
	// an empty function body and the noreturn attribute.
#ifdef __GNUC__ // GCC, Clang, ICC
	__builtin_unreachable();
#elifdef _MSC_VER // MSVC
	__assume(false);
#endif
}
