cmake_minimum_required(VERSION 3.20)
project(blur_parallel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV REQUIRED)

add_executable(blur main.cpp gaussianblur.cpp)
target_include_directories(blur PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(blur PRIVATE ${OpenCV_LIBS})
