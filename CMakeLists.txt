cmake_minimum_required(VERSION 4.0)
project(Pr_ctica_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Pr_ctica_1
        Especie.cpp
        Especie.h
        LectorCSV.cpp
        LectorCSV.h
        main.cpp
        VDinamico.h)
