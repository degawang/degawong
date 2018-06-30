
#include <iostream>
#include "cMemoExce.h"
#include "cParaExce.h"
#include "cDegaException.h"
#include "cImageFilter.h"

#include <opencv2/opencv.hpp> 

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{  
    try {
        cv::Mat image = imread("./input/001.jpg");
        degawong::cImageFilter imageFilter;
        imageFilter.setImageData(image);
        imageFilter.setFilterModel(DEGA_FILTER_SHARP);
        cv::Mat outputImage = imageFilter.getFilterImage();
        imshow("image filter image sharp : ",outputImage);
        cv::waitKey(0);
    }
    catch (const degawong::cDegaException & exce) {
        std::cerr << exce.what() << std::endl;
        throw;
    }
}
