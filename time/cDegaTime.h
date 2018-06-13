#pragma once

#include "string"
#include "iostream"

#ifdef DEGA_PLATFORM_WINDOW
#include "Windows.h"
#endif // DEGA_PLATFORM_WINDOW
#ifdef DEGA_PLATFORM_LINUX
#include<sys/time.h>
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
		}
		inline
			double toc() {
#ifdef DEGA_PLATFORM_WINDOW
			QueryPerformanceCounter(&currTime);
			pastTime = (currTime.QuadPart - prevTime.QuadPart)*1.0 / timeFrequency.QuadPart;
			QueryPerformanceCounter(&prevTime);
#endif // DEGA_PLATFORM_WINDOW			
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

	};

}