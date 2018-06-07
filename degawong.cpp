
#include "mat.h"
#include "cDegaTime.h"
#include"opencv2/opencv.hpp"

using namespace cv;
using namespace std;
using namespace degawong;

int main()
{
	cDegaTime ttt;
	// mat section
	ttt.tic();
	degawong::mat<float> mat_1(4, 4, 1, 3);
	degawong::mat<float> mat_2(4, 4, 1, 6);
	mat_2.print();
	{
		degawong::mat<float> mat_3 = mat_1 + mat_2;
		mat_2 = mat_3 + 4;
		mat_2 = mat_3 + 5;
		mat_3.print();
	}
	
	mat_2.print();

	// opencv section
	Mat img = imread("e:\\image\\000.jpg");
	mat<float> mat_4 = img;
	// imshow("i am a good boy", img);
	// cvWaitKey(0);
	ttt.toc();
	ttt.displayTime();

	return 0;
}

