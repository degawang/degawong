
#include "vec.h"
#include "mat.h"
#include "matrix.h"
#include "cDegaGui.h"
#include "cDegaTime.h"
#include "cImageFilter.h"
#include "degaPredefine.h"
#include "cRecursiveFind.h"

#include "armadillo"
#include "Eigen/Dense"

#include "iostream"

#ifdef DEGA_3RD_PARTY
#ifdef DEGA_3RD_PARTY_DLIB
#include <dlib/image_io.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing/frontal_face_detector.h>
using namespace dlib;
#endif // DEGA_3RD_PARTY_DLIB
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
	cDegaGui gui;

	mat<int> image(5, 5, 1, 100);
	image.at<vec3i>(1, 1)[0] = 10;
	image.at<vec3i>(1, 1)[1] = 11;
	image.at<vec3i>(1, 1)[2] = 12;
	cout << image << endl;

	//degawong::vec3i v3;
	//v3.at(0) = 1000;
	//v3.at(1) = 2000;
	//v3.at(2) = 3000;
	//for (int i = 0; i < 3; i++) {
	//	cout << v3.at(i) << endl;
	//}

	/* armadillo */
	//arma::mat A(5, 5, arma::fill::randu);
	//cout << A << endl;

	/* eigen */
	//Eigen::MatrixXd m(2, 2);
	//Eigen::Vector4d vec4d(1.0, 2.0, 3.0, 4.0);
	//Eigen::MatrixXd matrixXd;
	//Eigen::Matrix3d matrix3d;
	//m(0, 0) = 1;
	//m(0, 1) = 2;
	//m(1, 0) = m(0, 0) + 3;
	//m(1, 1) = m(0, 0) * m(0, 1);
	//m << -1.5, 2.4,
	//	6.7, 2.0;
	//std::cout << m << std::endl << std::endl;

	/* recursive finding */
	//cRecursiveFind recuFind;
	//recuFind.setDirName("e:\\image");
	//auto imageList = recuFind.getImageNameList();
	//for (int i = 0; i < imageList.size(); i++) {
	//	std::cout << imageList.at(i).c_str() << std::endl;
	//}
	
	/* dega gui */
	//cDegaGui gui;
	//int paralist[2] = { 0, 0 };
	//Mat img = imread("e:\\image\\001.bmp");
	//gui.disImage(img, "image");	
	//gui.createTrackBar(200, 100, 500, on_ThreshChange, "lightness", "image", &(paralist[0]));
	//gui.createTrackBar(300, 100, 700, on_ThreshChange, "brightness", "image", &(paralist[1]));
	//gui.setNouseCallBack("image", img, defaultMouseCallBack);

	/* it works well in 64 bit system, uncertain work or not in win 32 */
	//cv::Mat mask = cv::imread("./input/mask.bmp", cv::IMREAD_GRAYSCALE);
	//cv::Mat image_1 = cv::imread("./input/001.jpg");
	//cv::Mat image_2 = cv::imread("./input/002.jpg");
	//cout<<image_1.size()<<endl<<image_2.size()<<endl;
	//degawong::cLaplaceBlending laplaceImage(image_1, image_2, mask, 3);
	//cv::Mat image = laplaceImage.examples();

	//gui.disImage(image, "image");
	//imwrite("./output/blendResult.bmp", image);

	/* dlib */
	//dlib::frontal_face_detector detector = get_frontal_face_detector();
	//dlib::image_window win;
	//dlib::array2d<unsigned char> img;
	//dlib::load_image(img, "e:\\image\\000.jpg");
	//dlib::pyramid_up(img);
	//std::vector<dlib::rectangle> dets = detector(img);
	//std::cout << "Number of faces detected: " << dets.size() << std::endl;
	//win.clear_overlay();
	//win.set_image(img);
	//win.add_overlay(dets, rgb_pixel(255, 0, 0));

	//cv::Mat image = imread("./input/001.jpg");
	//degawong::cImageFilter imageFilter;
	//imageFilter.setImageData(image);
	//imageFilter.setFilterModel(DEGA_FILTER_SHARP);
	//cv::Mat outputImage = imageFilter.getFilterImage();
	//gui.disImage(image, "image filter image sharp : ");

	//waitKey(0);
	return 0;
}