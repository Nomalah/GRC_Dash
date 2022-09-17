cmake_minimum_required(VERSION 3.18)

project(GRCDash LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}, Flags: ${CMAKE_CXX_FLAGS}")

# Allows including .qrc files in executable
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# find_package(qt CONFIG REQUIRED)

add_subdirectory(src)