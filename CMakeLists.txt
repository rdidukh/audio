cmake_minimum_required (VERSION 2.8.11)
project(audio)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE
	main.cpp
	WavFile.cpp
	LinuxPlaybackDevice.cpp
)	

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC ".")

target_link_libraries(${PROJECT_NAME} asound)
