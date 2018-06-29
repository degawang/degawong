#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/opencv.h>  
#include <iostream>
 
#include <opencv2/opencv.hpp> 
using namespace dlib;
using namespace std;

/* still can not work */

int main(int argc, char** argv)
{  
    try
    {
		string imageName = "e:\\000.jpg";
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor sp;
        deserialize(imageName) >> sp;
 
		image_window win;
        array2d<rgb_pixel> img;
        load_image(img, imageName);
        pyramid_up(img);

        std::vector<rectangle> dets = detector(img);
        cout << "Number of faces detected: " << dets.size() << endl;
 
        std::vector<full_object_detection> shapes;
        for (unsigned long j = 0; j < dets.size(); ++j)
        {
            full_object_detection shape = sp(img, dets[j]);
            shapes.push_back(shape);
			cv::Mat temp = dlib::toMat(img);
 
			for (int k = 0; k < 68; ++k){
				circle(temp, cvPoint(shapes[j].part(k).x(), shapes[j].part(k).y()), 3, cv::Scalar(0, 0, 255), -1);
			}
        }

        win.clear_overlay();
        win.set_image(img);

    }
    catch (exception& e)
    {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
	system("pause");
}
