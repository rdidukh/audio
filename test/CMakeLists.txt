cmake_minimum_required (VERSION 2.8.11)
project(unittest)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE
	./RingBufferTest.cpp
        ../source/RingBuffer.cpp
#        gtest-1.7.0/src/gtest-all.cc
#       gtest-1.7.0/src/gtest_main.cc
)	

add_subdirectory(gtest-1.7.0)

add_executable(${PROJECT_NAME} ${SOURCE})

add_dependencies(${PROJECT_NAME} gtest gtest_main)

target_include_directories(${PROJECT_NAME} PUBLIC "../include")
target_include_directories(${PROJECT_NAME} PUBLIC "gtest-1.7.0/include")
target_include_directories(${PROJECT_NAME} PUBLIC "gtest-1.7.0")
target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/gtest-1.7.0/libgtest.a")
target_link_libraries(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/gtest-1.7.0/libgtest_main.a")
target_link_libraries(${PROJECT_NAME} "pthread")
