cmake_minimum_required (VERSION 2.8.11)
project(audio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -DTESTMETHODS")

set(SOURCE
	source/main.cpp
	source/RingBuffer.cpp
)	

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

add_subdirectory(test)

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC "./include")

target_link_libraries(${PROJECT_NAME} asound)
