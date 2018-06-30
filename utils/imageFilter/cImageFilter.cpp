#include "cImageFilter.h"

namespace degawong {

/* edgeLight fliter */
cv::Mat cImageFilter::edgeLight(){

    int para_1 = 3;
    int para_2 = 0;
    float alpha = 0.5;

    cv::Mat edgeRes_x;
    cv::Mat edgeRes_y;
    cv::Mat outputMat(image.size(),image.type());

    cv::Mat kernel_x = cv::Mat_<int>(3,3) << (-1, 0 ,1,
            -para_1, para_2, para_1,
            -1, 0 ,1);
    cv::Mat kernel_y = cv::Mat_<int>(3,3) << (-1, -para_1 ,-1,
            0, para_2, 0,
            1, para_1 ,1);

    filter2D(image,edgeRes_x,image.depth(),kernel_x);
    filter2D(image,edgeRes_y,image.depth(),kernel_y);

    /* iterator */
    cv::Mat_<cv::Vec3b>::iterator itend_1 = edgeRes_x.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart_1 = edgeRes_x.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3b>::iterator itstart_2 = edgeRes_y.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3b>::iterator itstart = outputMat.begin<cv::Vec3b>();

    for (; itstart_1 != itend_1;  ++itstart, ++itstart_1, ++itstart_2) {
        for (size_t loop_i = 0; loop_i < image.channels(); loop_i++) {
            (*itstart)[loop_i] = (alpha * ((*itstart_1)[loop_i]) + (1 - alpha) * ((*itstart_2)[loop_i]));
        }
    }
    return cv::Mat(outputMat);
}

}