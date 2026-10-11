cmake_minimum_required(VERSION 3.20)
project(ThreadsDemo CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ThreadsDemo main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(ThreadsDemo PRIVATE Threads::Threads)