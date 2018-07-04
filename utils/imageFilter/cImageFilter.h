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
	inline void setFilterIntensity(const float& _intensity) {
		intensity = _intensity;
	}
	inline void setFilterModel(const int& _filterModel) {
		filterModel = _filterModel;
	}
	inline cv::Mat getFilterImage() {
		switch (filterModel){
			case DEGA_FILTER_EDGE: {
				return edgeLight();
			}
			case DEGA_FILTER_SHARP: {
				return imageSharp();
			}
			case DEGA_FILTER_EMBOSS: {
				return emboss();
			}
			case DEGA_FILTER_SKETCH: {
				return sketch();
			}
			case DEGA_FILTER_SPHERIZE: {
				return spherizeWrap();
			}
			case DEGA_FILTER_COMIC: {
				return comic();
			}
			case DEGA_FILTER_LIGHT: {
				return light();
			}
			case DEGA_FILTER_WOODEN: {
				return wooden();
			}
			case DEGA_FILTER_OILPAINT: {
				return oilPaint();
			}
		}
	}
private:
	cv::Mat comic();
	cv::Mat light();
	cv::Mat wooden();
	cv::Mat emboss();
	cv::Mat sketch();
	cv::Mat oilPaint();
	cv::Mat edgeLight();
	cv::Mat imageSharp();
    cv::Mat spherizeWrap();
private:
	cv::Mat image;
	int filterModel;
	float intensity;
};

}