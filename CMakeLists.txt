cmake_minimum_required(VERSION 3.16)
project(goob)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
    ./src/main.cpp
    ./src/tokeniser.cpp
    ./src/tokeniser.hpp
    ./src/interpreter.cpp
    ./src/interpreter.hpp
)

make_directory(${CMAKE_BINARY_DIR}/lib)
make_directory(${CMAKE_BINARY_DIR}/build)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_executable(goob ${SOURCES})