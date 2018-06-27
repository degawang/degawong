#pragma once

#include "vector"
#include "iostream"
#include "corecrt_io.h"

namespace degawong {

class cRecursiveFind {
public:
	cRecursiveFind();
	virtual ~cRecursiveFind();

public:
	inline std::vector<std::string> recursiveFineImage(std::string dirName, std::vector<std::string>imageNameList) {

		try {
			_finddata_t imageInfo;
			std::string findTarget = dirName + "\\*";
			long imageHandle = _findfirst(findTarget.c_str(), &imageInfo);

			do {
				if (-1L == imageHandle) {
					throw;
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
		} catch (...) {
			std::cerr << "can not match the folder path" << std::endl;
		}
		return imageNameList;
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