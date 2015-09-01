#pragma once

#include <cstddef>

struct RingBuffer
{
    explicit RingBuffer(size_t bufferSize);

    size_t free() const;
    size_t available() const;
    size_t write(const char * buffer, size_t count);

	template<class T> 
    size_t write(const T & t);

    size_t read(char * buffer, size_t count);

	template<class T> 
    size_t read(T & t);

	char * data;
	size_t size;
	size_t left;
	size_t right;	
};
