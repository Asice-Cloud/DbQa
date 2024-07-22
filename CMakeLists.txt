cmake_minimum_required(VERSION 3.28)
project(DbQa)

set(CMAKE_CXX_STANDARD 23)

add_executable( DbQa
        Matrix.h
        Matrix.cpp
        Qbit.h
        Qbit.cpp
        Grover.h
        Grover.cpp
)
