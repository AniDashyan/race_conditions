cmake_minimum_required(VERSION 3.10)
project(race_conditions)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find threads
find_package(Threads REQUIRED)

add_executable(data_race main.cpp)