#include "cImageProcess.h"

namespace degawong {

/* image resize */
void cImageProcess::imResize(const cv::Mat inputImage, cv::Mat outputImage) {

    for (int loop_i =0; loop_i < outputImage.rows; loop_i++) {
        for (int loop_j =0; loop_j < outputImage.cols; loop_j++) {
            float weight_d = loop_j * ((float)inputImage.cols / (float)outputImage.cols);
            float weight_t = 1 - weight_t;
            float weight_r = loop_i * ((float)inputImage.rows / (float)outputImage.rows);
            float weight_l = 1 - weight_l;

            for (int loop_k = 0; loop_k < inputImage.channels(); loop_k++) {
                outputImage.at<cv::Vec3b>(loop_i, loop_j)[loop_k] = 0;
            }
        }
    }
}


}