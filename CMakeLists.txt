cmake_minimum_required(VERSION 3.11)
project(Boids CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(raylib REQUIRED)
add_executable(Boids main.cpp boid/boid.cpp boid/boid.h squoid/squoid.h squoid/squoid.cpp config.h)

target_link_libraries(Boids PRIVATE raylib)