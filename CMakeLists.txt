cmake_minimum_required(VERSION 3.22.1)

#project name
project (Engine)

set(CMAKE_BUILD_TYPE Debug)

find_package(raylib 4.2 REQUIRED)

file(GLOB SOURCES
    *.h
    *.cpp
    test/*.cpp
    test/*.h
)


add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} raylib)
