cmake_minimum_required (VERSION 3.8)

# hot reload for msvc
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("adventofcode-2023")
include_directories("src")

file(GLOB LIBRARY_FILES src/lib.h src/lib.cpp src/day*/solution*.cpp src/day*/solution*.h)
add_library(solutions ${LIBRARY_FILES})

# FIXME: generate this?
add_executable(day1 src/day1/main.cpp)
target_link_libraries(day1 solutions)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(day1_test "src/day1/test.cpp")
target_link_libraries(day1_test GTest::gmock_main)
target_link_libraries(day1_test solutions)
add_test(Day1Tests day1_test)

add_executable(day2_test "src/day2/test.cpp")
target_link_libraries(day2_test GTest::gmock_main)
target_link_libraries(day2_test solutions)
add_test(Day2Tests day2_test)

add_executable(day3_test "src/day3/test.cpp")
target_link_libraries(day3_test GTest::gmock_main)
target_link_libraries(day3_test solutions)
add_test(Day3Tests day3_test)

add_executable(day4_test "src/day4/test.cpp")
target_link_libraries(day4_test GTest::gmock_main)
target_link_libraries(day4_test solutions)
add_test(Day4Tests day4_test)

add_executable(day5_test "src/day5/test.cpp")
target_link_libraries(day5_test GTest::gmock_main)
target_link_libraries(day5_test solutions)
add_test(Day5Tests day5_test)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET solutions PROPERTY CXX_STANDARD 20)
    set_property(TARGET day1 PROPERTY CXX_STANDARD 20)
    set_property(TARGET day1_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day2_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day3_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day4_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET day5_test PROPERTY CXX_STANDARD 20)
endif()

# https://stackoverflow.com/questions/14306642/adding-multiple-executables-in-cmake
