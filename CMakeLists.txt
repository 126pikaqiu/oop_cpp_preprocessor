cmake_minimum_required(VERSION 3.13)
project(oop_cpp_preprocessor)

set(CMAKE_CXX_STANDARD 14)

add_executable(oop_cpp_preprocessor lab2.cpp Preprocessor.cpp Preprocessor.h Util.h Util.cpp Node.cpp Node.h)