# 3.16 to make use of target_precompile_headers
cmake_minimum_required(VERSION 3.16)

project(Duck2200-Assembler 
VERSION 0.1
LANGUAGES CXX)

option(CMAKE_BUILD_TYPE "The type of build" "DEBUG")
option(CMAKE_UNITY_BUILD "" OFF)

add_subdirectory(src)