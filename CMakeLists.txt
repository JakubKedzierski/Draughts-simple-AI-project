cmake_minimum_required(VERSION 3.10)

project(Draughts)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include/)
link_directories(include/)

file(GLOB SOURCES "src/*.cpp")
add_executable(Draugths ${SOURCES})
