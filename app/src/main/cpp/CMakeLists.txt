cmake_minimum_required(VERSION 3.22.1)

project("antirelib")

add_library(${CMAKE_PROJECT_NAME} SHARED
    antirelib.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log)