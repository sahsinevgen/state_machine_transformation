cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_STANDART 17)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(gtests tests.cpp)
add_subdirectory(libs)
target_link_libraries(gtests libs)

target_link_libraries(gtests ${GTEST_LIBRARIES} pthread)
