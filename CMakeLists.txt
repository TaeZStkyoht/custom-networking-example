cmake_minimum_required(VERSION 3.20)

project(custom-networking-example)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wconversion \
					-Wold-style-cast \
					-Wswitch-default \
					-Wduplicated-cond \
					-Wnull-dereference \
					-Wno-unknown-pragmas \
					-fno-exceptions \
")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
