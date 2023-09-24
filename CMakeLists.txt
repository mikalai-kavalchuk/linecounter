cmake_minimum_required(VERSION 3.10)
project(LineCounter)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(linecounter main.cpp LinesCounter.cpp)

target_link_libraries(linecounter Threads::Threads)