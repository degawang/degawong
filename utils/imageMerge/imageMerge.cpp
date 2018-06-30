
#include <iostream>
#include "cMemoExce.h"
#include "cParaExce.h"
#include "cDegaException.h"
#include "cImageMerge.h"

#include <opencv2/opencv.hpp> 

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{  
    try {
        cv::Mat image_1 = imread("./input/001.jpg");
        cv::Mat image_2 = imread("./input/002.jpg");
        degawong::cImageMerge imageMerge;
        imageMerge.setImageData(image_1,image_2);
        imageMerge.setMergeModel(DEGA_MERGE_SOFTLIGHT);
        cv::Mat outputImage = imageMerge.getMergeImage();
        imshow("image merge overlay : ",outputImage);
        cv::waitKey(0);
    }
    catch (const degawong::cDegaException & exce) {
        std::cerr << exce.what() << std::endl;
        throw;
    }
}
