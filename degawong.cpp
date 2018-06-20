
#include "iostream"
#include "mat.h"
#include "matrix.h"
#include "cDegaGui.h"
#include "cDegaTime.h"

#ifdef DEGA_3RD_PARTY
#include "armadillo"
#endif // DEGA_3RD_PARTY

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
using namespace degawong;

int a = 1;

void on_ThreshChange(int value, void* paraList)
{
	int *para = (int*)paraList;
	*para = value;
	cv::Mat image(512, 512, CV_8UC3);
	image = *para;
	cv::imshow("i am a good boy", image);
}


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
	//arma::mat A(5, 5, arma::fill::randu);
	//cout << A << endl;

	Mat img = imread("e:\\image\\000.jpg");

	matrix<float> matrix_1(4, 4, 1);
	matrix<float> matrix_2(4, 4, 1);
	matrix_2.print();
	{
		matrix<float> matrix_3 = matrix_1 + matrix_2;
		matrix_2 = matrix_3 + 4;
		matrix_2 = matrix_3 + 5;
		matrix_3.print();
	}
	matrix_2.print();
	
	imshow("image", img);
	int barPosition = 0;

	int aa[2] = { 0, 0 };
	cDegaGui gui;
	gui.createTrackBar(200, 100, 500, on_ThreshChange, "lightness", "image", &(aa[0]));
	gui.createTrackBar(300, 100, 700, on_ThreshChange, "brightness", "image", &(aa[1]));
	//createTrackbar("canny阀值", "imgoodboy", &barPosition, 255, on_ThreshChange);
	waitKey(0);


	return 0;
}