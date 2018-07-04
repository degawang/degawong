
#include <iostream>
#include "cDegaTime.h"
#include "cMemoExce.h"
#include "cParaExce.h"
#include "cDegaException.h"
#include "cImageProcess.h"

#include <opencv2/opencv.hpp> 

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{  
    try {
        degawong::cDegaGui gui;
        degawong::cDegaTime time;
        cv::Mat image = imread("e:/image/001.jpg",IMREAD_GRAYSCALE);
        imshow("ori_image",image);
        degawong::cImageProcess imageProcess;
        cv::Mat outputImage(image.rows * 2, image.cols * 3, image.type());
        imageProcess.getProcessImage(image,outputImage);
        imshow("image process image resize : ",outputImage);
        cv::waitKey(0);
    }
    catch (const degawong::cDegaException & exce) {
        std::cerr << exce.what() << std::endl;
        throw;
    }
}
