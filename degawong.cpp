
#include "iostream"
#include "mat.h"
#include "matrix.h"
#include "cDegaGui.h"
#include "cDegaTime.h"

#ifdef DEGA_3RD_PARTY
#include "armadillo"
#endif // DEGA_3RD_PARTY

#include "opencv2/opencv.hpp"
#include "cLaplaceBlending.h"

using namespace cv;
using namespace std;
using namespace degawong;

void on_ThreshChange(int value, void* paraList)
{
	int *para = (int*)paraList;
	*para = value;
	cv::Mat image(512, 512, CV_8UC3);
	image = *para;
	cv::imshow("i am a good boy", image);
}

cv::Rect rectToDraw;
int leftButtonDownFlag = false;

void defaultMouseCallBack(int eventHandle, int location_x, int location_y, int flags, void* inputPara) {
	cv::Mat* image = (cv::Mat*)inputPara;
	switch (eventHandle) {
	case CV_EVENT_MOUSEMOVE:
	{
		if (leftButtonDownFlag) {
			rectToDraw.width = location_x - rectToDraw.x;
			rectToDraw.height = location_y - rectToDraw.y;
		}
	}break;
	case CV_EVENT_LBUTTONDOWN:
	{
		leftButtonDownFlag = true;
		rectToDraw = cv::Rect(location_x, location_y, 0, 0);
	}break;
	case CV_EVENT_MBUTTONDOWN:
	{
		/* mid wheel down */
	}break;
	case CV_EVENT_MOUSEWHEEL:
	{
		int direction = getMouseWheelDelta(flags);
		/* mid wheel move up */
		if (0 < direction) {

		}
		/* mid wheel move down */
		else {

		}
	}break;
	case CV_EVENT_MBUTTONUP:
	{
		/* mid wheel up */
	}break;
	case CV_EVENT_LBUTTONUP:
	{
		leftButtonDownFlag = false;
		if (rectToDraw.width < 0) {
			rectToDraw.x += rectToDraw.width;
			rectToDraw.width *= -1;
		}
		if (rectToDraw.height < 0) {
			rectToDraw.y += rectToDraw.height;
			rectToDraw.height *= -1;
		}
		// drawing
		//draw_box(image, box);
	}break;
	}
}

int main()
{
	//arma::mat A(5, 5, arma::fill::randu);
	//cout << A << endl;

	//mat<float> mat_1(4, 4, 1, 3);
	//mat<float> mat_2(4, 4, 1, 6);
	//mat_2.print();
	//{
	//	mat<float> mat_3 = mat_1 + mat_2;
	//	mat_2 = mat_3 + 4;
	//	mat_2 = mat_3 + 5;
	//	mat_3.print();
	//}

	//matrix<float> matrix_1(4, 4, 1);
	//matrix<float> matrix_2(4, 4, 1);
	//matrix_2.print();
	//{
	//	matrix<float> matrix_3 = matrix_1 + matrix_2;
	//	matrix_2 = matrix_3 + 4;
	//	matrix_2 = matrix_3 + 5;
	//	matrix_3.print();
	//}
	//matrix_2.print();
	
	//cDegaGui gui;
	//int paralist[2] = { 0, 0 };
	//Mat img = imread("e:\\image\\001.bmp");
	//gui.disImage(img, "image");	
	//gui.createTrackBar(200, 100, 500, on_ThreshChange, "lightness", "image", &(paralist[0]));
	//gui.createTrackBar(300, 100, 700, on_ThreshChange, "brightness", "image", &(paralist[1]));
	//gui.setNouseCallBack("image", img, defaultMouseCallBack);

	cv::Mat mask = cv::imread("e:\\image\\mask.bmp", cv::IMREAD_GRAYSCALE);
	cv::Mat image_1 = cv::imread("e:\\image\\001.bmp");
	cv::Mat image_2 = cv::imread("e:\\image\\002.bmp");

	//cv::seamlessClone()

	degawong::cLaplaceBlending laplaceImage(image_1, image_2, mask, 1);
	cv::Mat image = laplaceImage.blendExamples();
	imshow("image", image);


	waitKey(0);
	return 0;
}