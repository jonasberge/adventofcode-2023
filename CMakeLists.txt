cmake_minimum_required (VERSION 3.8)

# hot reload for msvc
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("adventofcode-2023")
include_directories("lib")

file(GLOB LIBRARY_FILES lib/lib.h lib/lib.cpp day*/solution*.cpp day*/solution*.h)
add_library(solutions ${LIBRARY_FILES})
add_library(lib lib/lib.h lib/lib.cpp)

# FIXME: generate this?
add_executable(day1 day01/main.cpp)
target_link_libraries(day1 solutions)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(day1_test "day01/test.cpp")
target_link_libraries(day1_test GTest::gmock_main)
target_link_libraries(day1_test solutions)
add_test(Day1Tests day1_test)

add_executable(day2_test "day02/test.cpp")
target_link_libraries(day2_test GTest::gmock_main)
target_link_libraries(day2_test solutions)
add_test(Day2Tests day2_test)

add_executable(day3_test "day03/test.cpp")
target_link_libraries(day3_test GTest::gmock_main)
target_link_libraries(day3_test solutions)
add_test(Day3Tests day3_test)

add_executable(day4_test "day04/test.cpp")
target_link_libraries(day4_test GTest::gmock_main)
target_link_libraries(day4_test solutions)
add_test(Day4Tests day4_test)

add_executable(day5_test "day05/test.cpp")
target_link_libraries(day5_test GTest::gmock_main)
target_link_libraries(day5_test solutions)
add_test(Day5Tests day5_test)

add_executable(day6_test "day06/test.cpp")
target_link_libraries(day6_test GTest::gmock_main)
target_link_libraries(day6_test solutions)
add_test(Day6Tests day6_test)

add_executable(day7_test "day07/test.cpp")
target_link_libraries(day7_test GTest::gmock_main)
target_link_libraries(day7_test solutions)
add_test(Day7Tests day7_test)

add_executable(day8_test "day08/test.cpp")
target_link_libraries(day8_test GTest::gmock_main)
target_link_libraries(day8_test solutions)
add_test(Day8Tests day8_test)

add_executable(day9_test "day09/test.cpp")
target_link_libraries(day9_test GTest::gmock_main)
target_link_libraries(day9_test solutions)
add_test(Day9Tests day9_test)

add_executable(day10_test "day10/test.cpp")
target_link_libraries(day10_test GTest::gmock_main)
target_link_libraries(day10_test solutions)
add_test(Day10Tests day10_test)

add_executable(day11 "day11/solve.cpp")
target_link_libraries(day11 GTest::gmock_main)
target_link_libraries(day11 lib)
add_test(Day11 day11)

add_executable(day12 "day12/solve.cpp")
target_link_libraries(day12 GTest::gmock_main)
target_link_libraries(day12 lib)
add_test(Day12 day12)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET solutions PROPERTY CXX_STANDARD 20)
    set_property(TARGET day1 PROPERTY CXX_STANDARD 20)
    set_property(TARGET day1_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day2_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day3_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day4_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day5_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day6_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day7_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day8_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day9_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day10_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day11 PROPERTY CXX_STANDARD 20)
    set_property(TARGET day12 PROPERTY CXX_STANDARD 20)
endif()

# https://stackoverflow.com/questions/14306642/adding-multiple-executables-in-cmake
