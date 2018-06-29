#pragma once

#include "vector"
#include "iostream"
#include "cDegaGui.h"
#include "cMemoExce.h"
#include "cParaExce.h"
#include "cDegaException.h"

#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>  

#include <opencv2/opencv.hpp> 

namespace degawong {

class cFaceOutline {
public:
	cFaceOutline();
	virtual ~cFaceOutline();

public: 
	inline void setImageData(const std::string & _imageName) {
		imageName = _imageName;
		dlib::load_image(imageData, _imageName);
	}
	inline std::vector<dlib::full_object_detection> getLandmarkList() {
		return faceLandmarkList;
	}

private:
	void locatLandmark() {
		pyramid_up(imageData);
		dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
		std::vector<dlib::rectangle> dets = detector(imageData);
		dlib::shape_predictor sp;
		dlib::deserialize(imageName) >> sp;

		for (int i = 0; i < dets.size(); i++) {
			dlib::full_object_detection shape = sp(imageData, dets[i]);
			faceLandmarkList.push_back(shape);
		}
	}
	void showLandmark() {
		cDegaGui gui;
		cv::Mat imageMat;
		for (int i = 0; i < faceLandmarkList.size(); i++) {
			imageMat = dlib::toMat(imageData);
			for (int k = 0; k < 68; ++k) {
				circle(imageMat, cvPoint(faceLandmarkList.at(i).part(k).x(), faceLandmarkList.at(i).part(k).y()), 3, cv::Scalar(0, 0, 255), -1);
			}
		}
		gui.disImage(imageMat, "faceLandmark");
	}
private:
	std::string imageName;
	dlib::array2d<dlib::rgb_pixel> imageData;
	std::vector<dlib::full_object_detection> faceLandmarkList;
};

}