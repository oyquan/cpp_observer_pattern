project(observerpattern_test CXX)
cmake_minimum_required(VERSION 2.8.12)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-D__LCC__)
else()
    # Need to disable the default compiler flags first
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    add_definitions(-std=c++11)
    add_definitions(-static-libstdc++)
    add_definitions(-static-libgcc)
endif()

file(GLOB headers ${PROJECT_SOURCE_DIR}/*.h)
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(observerpattern_test ${headers} ${sources})
set(LIBRARY_INCLUDE ${PROJECT_SOURCE_DIR}/../include)
include_directories(${LIBRARY_INCLUDE})
