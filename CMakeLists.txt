cmake_minimum_required(VERSION 3.19)
project(GaussSeq)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

add_executable(seq_gauss_2 main.cpp)