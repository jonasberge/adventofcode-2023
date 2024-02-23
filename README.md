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

### Day 3

- Found these idioms for operator overloading: https://stackoverflow.com/q/4421706
- Learned about the spaceship operator and "default" comparison operator implementations,
  which reduces the boilerplate of merely comparing all attributes

### Day 4

(nothing)

### Day 5

Part 2 was the first problem so far that required a better solution than just the naive one.

I implemented the solution (intersecting ranges instead of probing individual seeds/numbers),
but I was convinced for quite a while that I would have to add the non-intersecting parts
of the original range to the set of resulting ranges, e.g.:
Seeds 10-19 and mapping 10-15 -> 50-55 results in two ranges
10-15, which then becomes 50-55, and 16-19.
It only makes sense to add the range 16-19 to the set of resulting ranges,
in contradiction though, NOT adding them got me the right solution!

To be sure that something was wrong I wrote a test around [example 2](./day5/input/example2.txt),
(`day5::InputTests/Solution2_Example2`) which confirmed my suspicion.
