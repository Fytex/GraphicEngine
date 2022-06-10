cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")


project(project)
add_subdirectory(src/generator)
add_subdirectory(src/utils)
add_subdirectory(src/engine)
add_subdirectory(src/pugixml)
