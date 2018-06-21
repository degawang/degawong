#pragma once

#include "string"
#include "iostream"

#ifdef DEGA_PLATFORM_WINDOW
#include "Windows.h"
#endif // DEGA_PLATFORM_WINDOW
#ifdef DEGA_PLATFORM_LINUX
#include<sys/time.h>
class timeValue {
public:
	timeValue() : second(0), usecond(0){}
	~timeValue() {}
public:
	long  second;
	long  usecond;
};
struct timeZone {
public:
	timeZone() : minuteSwest(0), dstTime(0) {}
	~timeZone() {}
public:
	int minuteSwest;
	int dstTime;
};
#endif // DEGA_PLATFORM_LINUX

namespace degawong {

	/* the time value in the unit of second */
	class cDegaTime {
	public:
		cDegaTime() : pastTime(0) {
#ifdef DEGA_PLATFORM_WINDOW
			QueryPerformanceFrequency(&timeFrequency);
			QueryPerformanceCounter(&currTime);
			QueryPerformanceCounter(&prevTime);
#endif // DEGA_PLATFORM_WINDOW
		}
		virtual ~cDegaTime() {};

	public:		
		inline
			void tic() {			
#ifdef DEGA_PLATFORM_WINDOW
			QueryPerformanceCounter(&prevTime);
#endif // DEGA_PLATFORM_WINDOW
#ifdef DEGA_PLATFORM_LINUX
			gettimeofday(&prevTime, NULL);
#endif // DEGA_PLATFORM_LINUX
		}
		inline
			double toc() {
#ifdef DEGA_PLATFORM_WINDOW
			QueryPerformanceCounter(&currTime);
			pastTime = (currTime.QuadPart - prevTime.QuadPart)*1.0 / timeFrequency.QuadPart;
			QueryPerformanceCounter(&prevTime);
#endif // DEGA_PLATFORM_WINDOW		
#ifdef DEGA_PLATFORM_LINUX
			gettimeofday(&currTime, NULL);
			pastTime = (currTime.second - prevTime.second) + (currTime.usecond - prevTime.usecond) / 1e6;
			gettimeofday(&prevTime, NULL);
#endif // DEGA_PLATFORM_LINUX
			return pastTime;
		}
		inline
			void displayTime(const std::string message) {
			std::cout << message << "cost " << pastTime << "s" << std::endl;
		}
	private:
		double pastTime;
#ifdef DEGA_PLATFORM_WINDOW
		LARGE_INTEGER currTime;		
		LARGE_INTEGER prevTime;		
		LARGE_INTEGER timeFrequency;
#endif // DEGA_PLATFORM_WINDOW
#ifdef DEGA_PLATFORM_LINUX
		timeValue currTime;
		timeValue prevTime;
#endif // DEGA_PLATFORM_LINUX

	};

}