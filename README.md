# Advent of Code (2023)

C++ project to solve 2023's Advent of Code challenges and learn some more C++.

https://adventofcode.com/2023

## Goals

- Getting more comfortable with C++, its more intricate language features and its standard library
- Learning to use CMake as a build system to compile C++ executables and libraries
- Using Windows and Visual Studio to develop software, instead of only on Linux
- Perhaps learning to use some third-party C++ libraries
- Using GTest for tests and ensuring clean code that can be easily refactored

## Dev Log

### Day 1

- Created a C++ CMake Console Application in Visual Studio
- Learned how a CMakeLists.txt is structured
- Solved the first task: https://adventofcode.com/2023/day/1
- Wrote tests with GTest for all meaningful use cases, examples and solutions
- Refactored everything to have a better structure for future solutions
- Standard library features used: regular expressions, string streams

### Day 2

- Separate writing solutions into three steps:
	1. Reading lines from input files
	2. Parsing/Converting lines to meaningful data structure instances
	3. Using the well-formed data with the solving algorithm
