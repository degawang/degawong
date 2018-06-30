
#include "cLaplaceBlending.h"


namespace degawong {

	cLaplaceBlending::~cLaplaceBlending() {}

	/* gauss pyramid */
	void cLaplaceBlending::buildGaussianPyramid() {
		try {			
			if (1 >= laplacePyramid_1.size()) {
				throw cParaExce("unvaild parameter");
			}
			gaussPyramid.clear();
			cv::Mat currentImage = blendMask;
			gaussPyramid.push_back(currentImage);
			for (int i = 0; i < nPyramidLayers; i++) {
				cv::Mat _down;
				pyrDown(currentImage, _down, laplacePyramid_1[i + 1].size());
				gaussPyramid.push_back(_down);
				currentImage = _down;
			}
		}
		catch (const cMemoExce& exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}
	/* laplacian pyramid */
	void cLaplaceBlending::buildLaplacianPyramid(
		const cv::Mat& image,
		std::vector<cv::Mat>& imagePyramid) {

		try {
			if (1 >= nPyramidLayers) {
				throw cParaExce("unvaild parameter");
			}
			imagePyramid.clear();
			cv::Mat currenImage = image;

			for (int i = 0; i < nPyramidLayers; i++) {

				cv::Mat _down, _up;
				cv::pyrDown(currenImage, _down);
				cv::pyrUp(_down, _up, currenImage.size());
				imagePyramid.push_back(currenImage - _up);
				currenImage = _down;

			}
			imagePyramid.push_back(currenImage);
		} 
		catch (const cParaExce& exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}

	/* blend pyramid */
	void cLaplaceBlending::blendLaplacianPyramids() {
	
		blendPyramid.clear();

		for (int i = 0; i < nPyramidLayers + 1; i++) {
			cv::Mat bgrMask;
			cv::cvtColor(gaussPyramid.at(i), bgrMask, CV_GRAY2BGR);
			cv::Mat A = laplacePyramid_1.at(i).mul(bgrMask);
			cv::Mat antiMask = cv::Scalar(1.0, 1.0, 1.0) - bgrMask;
			cv::Mat B = laplacePyramid_2.at(i).mul(antiMask);
			blendPyramid.push_back(A + B);
		}
	}

	/* reconstruct blended image */
	cv::Mat cLaplaceBlending::reconstructImageFromPyramid() {
		
		cv::Mat currenImage = blendPyramid.at(nPyramidLayers);
		for (int i = nPyramidLayers - 1; i >= 0; i--) {
			cv::Mat up;			
			pyrUp(currenImage, up, blendPyramid.at(i).size());
			currenImage = up + blendPyramid.at(i);
		}
		return currenImage;
	}

	cv::Mat cLaplaceBlending::examples() {
		
		buildLaplacianPyramid(image_1, laplacePyramid_1);
		buildLaplacianPyramid(image_2, laplacePyramid_2);
		buildGaussianPyramid();
		blendLaplacianPyramids();
		return reconstructImageFromPyramid();

	}
}
