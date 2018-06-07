#pragma once

#include "iostream"

#ifdef DEGA_TIME_WINDOW
#include "Windows.h"
#endif // DEGA_TIME_WINDOW
#ifdef DEGA_TIME_LINUX
#include<sys/time.h>
#endif // DEGA_TIME_LINUX

namespace degawong {

	/* the time value in the unit of second */
	class cDegaTime {
	public:
		cDegaTime() : pastTime(0) {
#ifdef DEGA_TIME_WINDOW
			QueryPerformanceFrequency(&timeFrequency);
			QueryPerformanceCounter(&currTime);
			QueryPerformanceCounter(&prevTime);
#endif // DEGA_TIME_WINDOW
		}
		virtual ~cDegaTime() {};

	public:		
		inline
			void tic() {
			QueryPerformanceCounter(&prevTime);
		}
		inline
			double toc() {
			QueryPerformanceCounter(&currTime);
			pastTime = (currTime.QuadPart - prevTime.QuadPart)*1.0 / timeFrequency.QuadPart;
			QueryPerformanceCounter(&prevTime);
			return pastTime;
		}
		inline
			void displayTime() {
			std::cout << "cost " << pastTime << "s" << std::endl;
		}
	private:
		double pastTime;
		LARGE_INTEGER currTime;		
		LARGE_INTEGER prevTime;		
		LARGE_INTEGER timeFrequency;
	};

}