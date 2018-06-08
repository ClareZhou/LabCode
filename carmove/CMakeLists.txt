cmake_minimum_required(VERSION 3.5)
project(carmove)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -pthread ")
set(SRC test/main.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_EXE_LINKER_FLAGS "-pthread ")
set(SHARE_LIB_PREFIX /usr/local)
set(CMAKE_CXX_STANDARD 11)



link_directories(${LINK_DIRECTORIES})

add_subdirectory(test)

#add_executable(carmove test/main.cpp include/timeout.h include/car_zyl.h src/timeout.cpp include/uwblps.h src/uwblps.cpp
#        include/serial.h src/serial.cpp include/inf_typedef.h src/car_zyl.cpp)
#target_link_libraries(carmove pthread zmq curses)