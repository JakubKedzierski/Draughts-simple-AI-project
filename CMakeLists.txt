cmake_minimum_required(VERSION 3.10)

project(Draughts)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include/)
link_directories(include/)

find_package(SFML REQUIRED system window graphics network audio)
target_link_libraries(Draughts ${SFML_LIBRARIES} ${SFML_DEPENDENCIES} )

file(GLOB SOURCES "src/*.cpp")
add_executable(Draugths ${SOURCES})

