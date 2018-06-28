#pragma once

#include "algorithm"
#include "cMemoExce.h"
#include "cParaExce.h"
#include "cDegaException.h"
#include "opencv2/opencv.hpp"

namespace degawong {

class cLaplaceBlending {
public:
	cLaplaceBlending() : nPyramidLayers(3) {};
	
	virtual ~cLaplaceBlending();

	cLaplaceBlending(
		const cv::Mat& _image_1,
		const cv::Mat& _image_2,
		const cv::Mat& _blendMask,
		int _nPyramidLayers = 3) : nPyramidLayers(_nPyramidLayers) {

		try {
			if ((_image_1.size() != _image_2.size()) || (_image_1.size() != _blendMask.size())) {
				throw cParaExce("unmatched image");
			}

			_image_1.convertTo(image_1, CV_32F, 1.0 / 255.0);
			_image_2.convertTo(image_2, CV_32F, 1.0 / 255.0);
			_blendMask.convertTo(blendMask, CV_32F, 1.0 / 255.0);

			int minBorder = image_1.cols > image_1.rows ? image_1.rows : image_1.cols;
			nPyramidLayers =
				((log(minBorder) / log(2) - 4.0) > 1.0 ? (log(minBorder) / log(2) - 4.0) : 1.0) > nPyramidLayers ? nPyramidLayers :
				((log(minBorder) / log(2) - 4.0) > 1.0 ? (log(minBorder) / log(2) - 4.0) : 1.0);
		} 
		catch (const cParaExce& exce) {
			std::cerr << exce.getExceReason() << std::endl;
			throw;
		}
	};

public:
	void buildGaussianPyramid();

	void blendLaplacianPyramids();

	cv::Mat reconstructImageFromPyramid();

	void buildLaplacianPyramid(const cv::Mat& image,
		std::vector<cv::Mat>& imagePyramid);

public:
	cv::Mat examples();

private:
	int nPyramidLayers;

	cv::Mat image_1;
	cv::Mat image_2;
	cv::Mat blendMask;

	std::vector<cv::Mat> blendPyramid;
	std::vector<cv::Mat> gaussPyramid;
	std::vector<cv::Mat> laplacePyramid_1;
	std::vector<cv::Mat> laplacePyramid_2;
		
	};

}

