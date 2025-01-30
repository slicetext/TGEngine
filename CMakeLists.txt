cmake_minimum_required(VERSION 3.22.1)

#project name
project (Engine)

find_package(raylib 4.2 REQUIRED)

file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} raylib)
