cmake_minimum_required(VERSION 3.13)

project(Elevator)

SET(MYCOMPILE_FLAGS "-no-pie -pthread")

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${MYCOMPILE_FLAGS}")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${MYCOMPILE_FLAGS}")

set(SOURCES src/main.cpp src/elevator.cpp)
set(HEADERS src/elevator.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
