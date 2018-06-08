SET(SOURCE_FILES  ../include/timeout.h ../include/car_zyl.h ../src/timeout.cpp ../include/uwblps.h ../src/uwblps.cpp
        ../include/serial.h ../src/serial.cpp ../include/inf_typedef.h ../src/car_zyl.cpp )


LINK_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/lib
        /usr/local/lib
        /usr/lib
)

INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/include
        /usr/local/include
        /usr/include
)

ADD_EXECUTABLE(carmove main.cpp ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(carmove zmq pthread curses)

add_executable(sub carsub.cpp)
target_link_libraries(sub zmq pthread)

add_executable(pub mypub.cpp)
target_link_libraries(pub zmq pthread)

add_executable(keyboardcontrol keyboardcontrol.cpp ${SOURCE_FILES})
target_link_libraries(keyboardcontrol curses zmq pthread)

add_executable(testuwb testuwb.cpp ${SOURCE_FILES})
target_link_libraries(testuwb zmq pthread curses)

add_executable(run2 run2.cpp ${SOURCE_FILES})
target_link_libraries(run2 zmq pthread curses zmq pthread)
