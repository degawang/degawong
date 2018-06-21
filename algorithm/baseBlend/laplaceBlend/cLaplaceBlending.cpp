
#include "cLaplaceBlending.h"


namespace degawong {

	cLaplaceBlending::cLaplaceBlending() {
	}


	cLaplaceBlending::~cLaplaceBlending() {
	}

	//////////////////////////////////////////////////////////////////////////
	// gauss pyramid
	void cLaplaceBlending::buildGaussianPyramid() {

		assert(image_pyramid_1.size() > 0 && image_pyramid_2.size() > 0);

		gauss_pyramid_mask.clear();
		cv::Mat currentImage;
		cv::cvtColor(blendMask, currentImage, CV_GRAY2BGR);
		gauss_pyramid_mask.push_back(currentImage); 

		currentImage = blendMask;
		for (int i = 1; i < pyramidLayers + 1; i++) {
			cv::Mat _down;
			int para_size = image_pyramid_1.size();
			if (para_size > i)
				pyrDown(currentImage, _down, image_pyramid_1[i].size());
			else
				pyrDown(currentImage, _down, pyramid_top_1.size()); //lowest level

			cv::Mat down;
			cvtColor(_down, down, CV_GRAY2BGR);
			gauss_pyramid_mask.push_back(down);//add color blend mask into mask Pyramid
			currentImage = _down;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// laplacian pyramid
	void cLaplaceBlending::buildLaplacianPyramid(const cv::Mat& image,
		std::vector<cv::Mat_<cv::Vec3f> >& image_pyramid,
		cv::Mat& pyramid_top) 
	{
		image_pyramid.clear();
		cv::Mat currentImage = image;
		for (int i = 0; i < pyramidLayers; i++) {
			cv::Mat down, up;
			cv::pyrDown(currentImage, down);
			cv::pyrUp(down, up, currentImage.size());
			cv::Mat pyramid = currentImage - up;
			image_pyramid.push_back(pyramid);
			currentImage = down;
		}
		currentImage.copyTo(pyramid_top);
	}

	//////////////////////////////////////////////////////////////////////////
	// blend pyramid
	void cLaplaceBlending::blendLaplacianPyramids() {
		pyramid_top_res = pyramid_top_1.mul(gauss_pyramid_mask.back()) +
			pyramid_top_2.mul(cv::Scalar(1.0, 1.0, 1.0) - gauss_pyramid_mask.back());
		for (int i = 0; i < pyramidLayers; i++) {
			cv::Mat A = image_pyramid_1[i].mul(gauss_pyramid_mask[i]);
			cv::Mat antiMask = cv::Scalar(1.0, 1.0, 1.0) - gauss_pyramid_mask[i];
			cv::Mat tempMat = image_pyramid_2[i];
			cv::Mat B = tempMat.mul(antiMask);
			cv::Mat_<cv::Vec3f> blendedImage = A + B;

			image_pyramid_res.push_back(blendedImage);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// reconstruct blended image
	cv::Mat_<cv::Vec3f> cLaplaceBlending::reconstructImageFromPyramid() {
		cv::Mat currentImage = pyramid_top_res;
		for (int i = pyramidLayers - 1; i >= 0; i--) {
			cv::Mat up;
			pyrUp(currentImage, up, image_pyramid_res[i].size());
			currentImage = up + image_pyramid_res[i];
		}
		return currentImage;
	}

	cv::Mat cLaplaceBlending::blendExamples() {
		
		buildLaplacianPyramid(image_1, image_pyramid_1, pyramid_top_1);
		buildLaplacianPyramid(image_2, image_pyramid_2, pyramid_top_2);
		buildGaussianPyramid();
		blendLaplacianPyramids();
		return reconstructImageFromPyramid();

	}
}
