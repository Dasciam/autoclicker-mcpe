cmake_minimum_required(VERSION 3.20)
project(autoclicker_mcpe)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(args)

add_executable(autoclicker_mcpe main.cpp xutils.cpp)

target_link_libraries(autoclicker_mcpe X11 args)
