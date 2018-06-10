
#include "iostream"
#include "mat.h"
#include "cDegaGui.h"
#include "cDegaTime.h"

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
using namespace degawong;

int main()
{
	mat<float> mat_1(4, 4, 1, 3);
	mat<float> mat_2(4, 4, 1, 6);
	mat_2.print();
	{
		mat<float> mat_3 = mat_1 + mat_2;
		mat_2 = mat_3 + 4;
		mat_2 = mat_3 + 5;
		mat_3.print();
	}

	Mat img = imread("000.jpg");
	cDegaGui dega;
	dega.disImage(img, "imgoodboy");
	cvWaitKey(0);

	return 0;
}