cmake_minimum_required(VERSION 3.22.1)
include(FetchContent)

#project name
project (Engine)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".so" ".a")
SET(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

find_package(raylib 4.2 REQUIRED)
FetchContent_Declare(
	box2d
	GIT_REPOSITORY https://github.com/erincatto/box2d.git
	GIT_TAG main
	GIT_SHALLOW TRUE
	GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(box2d)
# find_package(${PROJECT_SOURCE_DIR}/include/box2d.a)

file(GLOB SOURCES
    include/*.h
    include/*.c
    include/*.cpp
    *.h
    *.cpp
    test/*.cpp
    test/*.h
)


add_executable(${PROJECT_NAME} ${SOURCES})

# set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)


target_link_libraries(${PROJECT_NAME} raylib box2d)
