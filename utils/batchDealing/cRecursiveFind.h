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
	inline std::vector<std::string> recursiveFineImage(std::string dirName, std::vector<std::string>imageNameList) {

#ifdef DEGA_PLATFORM_WINDOW
		try {
			_finddata_t imageInfo;
			std::string findTarget = dirName + "\\*";
			long imageHandle = _findfirst(findTarget.c_str(), &imageInfo);

			do {
				if (-1L == imageHandle) {
					throw cDegaException("error : can not match the folder path");
				}
				if (imageInfo.attrib & _A_SUBDIR) {
					if ((strcmp(imageInfo.name, ".") != 0) && (strcmp(imageInfo.name, "..") != 0)) {
						std::string subDirName = dirName + "\\" + imageInfo.name;
						recursiveFineImage(subDirName, imageNameList);
					}
				}
				else {
					imageNameList.push_back(imageInfo.name);
				}
			} while (0 == _findnext(imageHandle, &imageInfo));
			return imageNameList;
		} 
		catch (const cDegaException& exce) {
			std::cerr << exce.getExceReason() << std::endl;
			throw;
		}
#ifdef DEGA_PLATFORM_LINUX
	
#endif // DEGA_PLATFORM_LINUX
	}

public: 
	inline void setDirName(std::string dirName) {
		strDirName = dirName;
	}
	inline std::vector<std::string> getImageNameList() {
		return recursiveFineImage(strDirName, vecImageName);
	}
private:
	std::string strDirName;
	std::vector<std::string> vecImageName;
};

}