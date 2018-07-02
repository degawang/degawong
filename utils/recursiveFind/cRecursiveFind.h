#pragma once

#include "vector"
#include "iostream"
#include "cMemoExce.h"
#include "cParaExce.h"
#include "cDegaException.h"

#ifdef DEGA_PLATFORM_WINDOW
#include "corecrt_io.h"
#endif // DEGA_PLATFORM_WINDOW
#ifdef DEGA_PLATFORM_LINUX
#include<sys/time.h>
#endif // DEGA_PLATFORM_LINUX


namespace degawong {

class cRecursiveFind {
public:
	cRecursiveFind();
	virtual ~cRecursiveFind();

public:
	inline std::vector<std::string> recursiveFineImage(const std::string inputDir, std::vector<std::string> imageList) {
		try {
		/* can not work in win 64,why? */
#ifdef DEGA_PLATFORM_WINDOW
			_finddata_t imageInfo;
			std::string findTarget = inputDir + "\\*";
			long imageHandle = _findfirst(findTarget.c_str(), &imageInfo);

			do {
				if (-1L == imageHandle) {
					throw cDegaException("error : can not match the folder path");
				}
				if (imageInfo.attrib & _A_SUBDIR) {
					if ((strcmp(imageInfo.name, ".") != 0) && (strcmp(imageInfo.name, "..") != 0)) {
						std::string subDirName = inputDir + "\\" + imageInfo.name;
						recursiveFineImage(subDirName, imageList);
					}
				}
				else {
					imageList.push_back(imageInfo.name);
				}
			} while (0 == _findnext(imageHandle, &imageInfo));		
#endif // DEGA_PLATFORM_WINDOW

#ifdef DEGA_PLATFORM_LINUX
	
#endif // DEGA_PLATFORM_LINUX
			return imageList;
		} 
		catch (const cDegaException& exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}

public: 
	inline void setInputDir(const std::string & _inputDir) {
		inputDir = _inputDir;
	}
	inline void setOutputDir(const std::string & _outputDir) {
		outputDir = _outputDir;
	}
	inline std::vector<std::string> getImageList() {
		return recursiveFineImage(inputDir, imageList);
	}
private:
	std::string inputDir;
	std::string outputDir;
	std::vector<std::string> imageList;
};

}