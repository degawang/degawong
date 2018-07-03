#pragma once

#include "vector"
#include "iostream"
#include "cDegaGui.h"
#include "cMemoExce.h"
#include "cParaExce.h"
#include "cDegaException.h"
#include "degaPredefine.h"

#include <opencv2/opencv.hpp> 

namespace degawong {

class cImageProcess {
public:
	cImageProcess() : processModel(0) {}
    cImageProcess(int _processModel) : processModel(_processModel) {}
	virtual ~cImageProcess() {}

public:
	inline void setProcessModel(const int & _processModel) {
		processModel = _processModel;
	}
	inline cv::Mat getProcessImage(const cv::Mat inputImage, cv::Mat outputImage) {
		switch (processModel){
			case DEGA_PROCESS_RESIZE:{
                imResize(inputImage, outputImage);
			}
		}
	}
private:
	inline void imResize(const cv::Mat inputImage, cv::Mat outputImage);
private:
	int processModel;
};

}