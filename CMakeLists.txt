cmake_minimum_required(VERSION 3.14)
project(cpp_project_template)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(tests)

enable_testing()