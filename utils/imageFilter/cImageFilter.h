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

class cImageFilter {
public:
	cImageFilter() : filterModel(0) {}
	virtual ~cImageFilter() {}

public: 
	inline void setImageData(const cv::Mat & _image) {
		image = _image;
	}
	inline void setFilterModel(const int & _filterModel) {
		filterModel = _filterModel;
	}
	inline cv::Mat getFilterImage() {
		switch (filterModel){
			case DEGA_FILTER_EDGE:{
				return edgeLight();
			}
			case DEGA_FILTER_SHARP:{
				return imageSharp();
			}
		}
	}
private:
	cv::Mat edgeLight();
	cv::Mat imageSharp();
private:
	cv::Mat image;
	int filterModel;
};

}