cmake_minimum_required(VERSION 3.14)
project(Project2)

set(CMAKE_CXX_STANDARD 17)

include_directories($ENV{HOME}/Lab6/include)
link_directories($ENV{HOME}/Lab6/lib)


add_executable(Project2 main.cpp dictionary.cpp dictionary.h fraction.cc fraction.h stack.h)