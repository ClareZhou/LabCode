cmake_minimum_required(VERSION 3.9)
project(carmove)

set(CMAKE_CXX_STANDARD 11)

add_executable(carmove main.cpp timeout.h car_zyl.h timeout.cpp uwblps.h uwblps.cpp serial.h serial.cpp inf_typedef.h car_zyl.cpp)