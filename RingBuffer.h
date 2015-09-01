#include <cstring>

struct RingBuffer
{
	RingBuffer(size_t bufferSize): buffer(new char[bufferSize]), size(bufferSize), left(0), right(0)
	{

	}	

	size_t free()
	{
		return size - (right - left);
	}

	size_t available()
	{
		return right - left;
	}

	size_t write(const char * buffer, size_t count)
	{
		if(count > free())
			count = free();

		if(count + right <= size)
		{
			std::memcpy(data[right], buffer, count);
		}
		else
		{
			std::memcpy(&data[right], buffer, size-right);
			std::memcpy(&data[0], &buffer[size-right], count - (size-right));
		}

		right += count;

		return count;
	}

	template<class T> 
	size_t write(const T & t)
	{
		return write((const char *)&t, sizeof(t));
	}

	size_t read(char * buffer, size_t count)
	{
		if(count > available())
			count = available();

		if(left + count <= size)
		{
			std::memcpy(buffer, &data[left], count);
		}		
		else
		{
			std::memcpy(buffer, &data[left], size-left);
			std::memcpy(&buffer[size-left], &data[0], count-(size-left));
		}

		left += count;
	
		if(left >= size)
		{
			left -= size;
			right -= size;
		}

		return count;
	}

	template<class T> 
	size_t read(T & t)
	{
		return read((char *)&t, sizeof(t));
	}

	private:
	char * data;
	size_t size;
	size_t left;
	size_t right;	

};
