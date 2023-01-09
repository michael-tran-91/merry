# The following lines of boilerplate have to be in your project's CMakeLists
# in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.1)

project(merry)

file(GLOB SRC_FILES object/*.c)

add_executable(merry main.c ${SRC_FILES})
