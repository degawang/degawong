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

class cImageMerge {
public:
	cImageMerge() : mergeModel(0) {}
	virtual ~cImageMerge() {}

public:
	inline void setMergeModel(const int & _mergeModel) {
		mergeModel = _mergeModel;
	}
	inline void setImageData(const cv::Mat & _image_1, const cv::Mat & _image_2) {
		image_1 = _image_1;
		image_2 = _image_2;
	}
	inline cv::Mat getMergeImage() {
		switch (mergeModel){
			case DEGA_MERGE_HARDMIX:{
				return hardmix();
			}
			case DEGA_MERGE_OVERLAY:{
				return overlay();
			}
			case DEGA_MERGE_PINLIGHT:{
				return pinlight();
			}
			case DEGA_MERGE_HARDLIGHT:{
				return hardlight();
			}
			case DEGA_MERGE_SOFTLIGHT:{
				return softlight();
			}
			case DEGA_MERGE_LINERLIGHT:{
				return linerlight();
			}
			case DEGA_MERGE_VIVLDLIGHT:{
				return vividlight();
			}
		}
	}

private:
	cv::Mat hardmix();
	cv::Mat overlay();
    cv::Mat pinlight();
	cv::Mat hardlight();
	cv::Mat softlight();
	cv::Mat linerlight();
	cv::Mat vividlight();

private:
	int mergeModel;
	cv::Mat image_1;
	cv::Mat image_2;
};

}