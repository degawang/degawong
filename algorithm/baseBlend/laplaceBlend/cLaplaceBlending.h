#pragma once

#include "algorithm"
#include "opencv2/opencv.hpp"

namespace degawong {

	class cLaplaceBlending {
	public:
		cLaplaceBlending();
		virtual ~cLaplaceBlending();

	private:
		int pyramidLayers;

		cv::Mat u_blendMask;
		cv::Mat_<cv::Vec3b> u_image_1;
		cv::Mat_<cv::Vec3b> u_image_2;

		cv::Mat_<float> blendMask;
		cv::Mat_<cv::Vec3f> image_1;
		cv::Mat_<cv::Vec3f> image_2;

		cv::Mat_<cv::Vec3f> pyramid_top_1;
		cv::Mat_<cv::Vec3f> pyramid_top_2;
		cv::Mat_<cv::Vec3f> pyramid_top_res;

		std::vector<cv::Mat_<cv::Vec3f> > image_pyramid_1;
		std::vector<cv::Mat_<cv::Vec3f> > image_pyramid_2;
		std::vector<cv::Mat_<cv::Vec3f> > image_pyramid_res;

		std::vector<cv::Mat_<cv::Vec3f> > gauss_pyramid_mask;

		void buildGaussianPyramid();

		void buildLaplacianPyramid(const cv::Mat& image,
			std::vector<cv::Mat_<cv::Vec3f> >& image_pyramid,
			cv::Mat& top_pyramid);

		void blendLaplacianPyramids();

		cv::Mat_<cv::Vec3f> reconstructImageFromPyramid();

	public:
		cLaplaceBlending(const cv::Mat_<cv::Vec3f>& _image_1,
			const cv::Mat_<cv::Vec3f>& _image_2,
			const cv::Mat_<float>& _blendMask,
			int _pyramidLayers = 3) :
			u_image_1(_image_1),
			u_image_2(_image_2),
			u_blendMask(_blendMask),
			pyramidLayers(_pyramidLayers) {

			assert(_image_1.size() == _image_2.size());
			assert(_image_1.size() == _blendMask.size());

			u_blendMask.convertTo(blendMask, CV_32F, 1.0 / 255.0);
			u_image_1.convertTo(image_1, CV_32F, 1.0 / 255.0);
			u_image_2.convertTo(image_2, CV_32F, 1.0 / 255.0);

			int minBorder = image_1.cols > image_1.rows ? image_1.rows : image_1.cols;
			double value_1 = (log(minBorder) / log(2) - 4.0) > 1.0 ? (log(minBorder) / log(2) - 4.0) : 1.0;
			pyramidLayers = value_1 > pyramidLayers ? pyramidLayers : value_1;			
		};

		cv::Mat blendExamples();
	};

}

