

#include "cDegaGui.h"
#include "cRecursiveFind.h"
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;
using namespace degawong;

int main() 
{
	try{
		cRecursiveFind recuFind;
		recuFind.setDirName("e:\\image");
		auto imageList = recuFind.getImageNameList();
		for (int i = 0; i < imageList.size(); i++) {
			std::cout << imageList.at(i).c_str()<< std::endl;
		}
		cDegaGui gui;
		Mat image = imread("e:\\image\\001.jpg");
		gui.disImage(image);
		waitKey(0);
	}
	catch (...) {

	}

	return 0;
}