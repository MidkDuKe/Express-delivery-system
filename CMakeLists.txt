cmake_minimum_required(VERSION 3.28)
project(ExpressProject C)

set(CMAKE_C_STANDARD 11)

add_executable(ExpressProject main.c
        ExpressStruct.h
        ExpressInfo.c
        Graph.h
        Graph.c
)
