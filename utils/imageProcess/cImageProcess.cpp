#include "cImageProcess.h"

namespace degawong {

/* image resize */
void cImageProcess::imResize(const cv::Mat inputImage, cv::Mat outputImage) {

    for (int loop_i =0; loop_i < outputImage.rows; loop_i++) {
        for (int loop_j =0; loop_j < outputImage.cols; loop_j++) {

			int borderVertical = 1;
			int borderHorizontal = 1;

			if ((loop_i * floor(inputImage.cols / outputImage.cols)) >= inputImage.cols - 1) {
				borderHorizontal = -1;
			}
			else {
				borderHorizontal = 1;
			}

			if ((loop_j * floor(inputImage.rows / outputImage.rows)) >= inputImage.rows - 1) {
				borderVertical = -1;
			}
			else {
				borderVertical = 1;
			}

			float locat_l = loop_i * floor(inputImage.cols / outputImage.cols);
			float locat_r = locat_l + borderHorizontal;
			float locat_t = loop_j * floor(inputImage.rows / outputImage.rows);
			float locat_d = locat_t + borderVertical;

            float weight_d = loop_j * ((float)inputImage.cols / (float)outputImage.cols);
            float weight_t = 1 - weight_t;
            float weight_r = loop_i * ((float)inputImage.rows / (float)outputImage.rows);
            float weight_l = 1 - weight_l;

			int leftTopValue = inputImage.at<uchar>(locat_l, locat_t);
			int rightTopValue = inputImage.at<uchar>(locat_r, locat_t);
			int leftDownValue = inputImage.at<uchar>(locat_l, locat_d);
			int rightDownValue = inputImage.at<uchar>(locat_r, locat_d);

			outputImage.at<uchar>(loop_i, loop_j) =
				weight_l * weight_t * leftTopValue + weight_r * weight_t * rightTopValue + weight_l * weight_d * leftDownValue + weight_r * weight_d * rightDownValue;
    //        for (int loop_k = 0; loop_k < inputImage.channels(); loop_k++) {
				//int leftValue = inputImage.at<cv::Vec3b>(loop_i, loop_j)[0];
    //            outputImage.at<cv::Vec3b>(loop_i, loop_j)[loop_k] = 0;
    //        }
        }
    }
}


}