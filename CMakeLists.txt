cmake_minimum_required (VERSION 3.8)

# hot reload for msvc
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("adventofcode-2023")

file(GLOB LIBRARY_FILES lib.h lib.cpp day*.h day*.cpp)
add_library(solutions_lib ${LIBRARY_FILES})

# add_executable(day1_solution1 "day1_solution1.cpp" "lib.cpp" "lib.h" "day1_solution1.h")
add_executable(solutions solutions.cpp)
target_link_libraries(solutions solutions_lib)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests "test.cpp" "day1_solution1.h")
target_link_libraries(tests ${GTEST_BOTH_LIBRARIES})
target_link_libraries(tests solutions_lib)
add_test(Tests tests)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET solutions_lib PROPERTY CXX_STANDARD 20)
    set_property(TARGET solutions PROPERTY CXX_STANDARD 20)
    set_property(TARGET tests PROPERTY CXX_STANDARD 20)
endif()
