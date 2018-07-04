#include "cImageFilter.h"

namespace degawong {

/* sketch filter */
cv::Mat cImageFilter::sketch() {

	cv::Mat imageGray;
	cv::Mat imageGauss;
	cv::Mat outputMat(image.size(), CV_8UC1);
	cv::cvtColor(image, imageGray, cv::COLOR_BGR2GRAY);
	cv::Mat grayReverse = 255 - imageGray;
	cv::Mat kernel = (cv::Mat_<int>(3, 3) << 1, 2, 1,
		2, 4, 2,
		1, 2, 1);
	cv::filter2D(grayReverse, imageGauss, grayReverse.depth(), kernel);

	cv::Mat_<uchar>::iterator itend_1 = imageGray.end<uchar>();
	cv::Mat_<uchar>::iterator itstart_1 = imageGray.begin<uchar>();
	cv::Mat_<uchar>::iterator itstart_2 = imageGauss.begin<uchar>();
	cv::Mat_<uchar>::iterator itstart = outputMat.begin<uchar>();

	for (; itstart_1 != itend_1; ++itstart, ++itstart_1, ++itstart_2) {
		for (size_t loop_i = 0; loop_i < image.channels(); loop_i++) {
			float grayValue = *itstart_1;
			float gaussValue = *itstart_2;
			
			*itstart = uchar(std::min(255.f,grayValue + grayValue * gaussValue / (255.f - gaussValue)));
		}
	}
	return cv::Mat(outputMat);
}
/* edgeLight filter */
cv::Mat cImageFilter::edgeLight(){

    int para_1 = 3;
    int para_2 = 0;
    float alpha = 0.5;

    cv::Mat edgeRes_x;
    cv::Mat edgeRes_y;
    cv::Mat outputMat(image.size(),image.type());

    cv::Mat kernel_x = cv::Mat_<int>(3,3) << (-1, 0 ,1,
            -para_1, para_2, para_1,
            -1, 0 ,1);
    cv::Mat kernel_y = cv::Mat_<int>(3,3) << (-1, -para_1 ,-1,
            0, para_2, 0,
            1, para_1 ,1);

    filter2D(image,edgeRes_x,image.depth(),kernel_x);
    filter2D(image,edgeRes_y,image.depth(),kernel_y);

    /* iterator */
    cv::Mat_<cv::Vec3b>::iterator itend_1 = edgeRes_x.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart_1 = edgeRes_x.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3b>::iterator itstart_2 = edgeRes_y.begin<cv::Vec3b>();

    cv::Mat_<cv::Vec3b>::iterator itstart = outputMat.begin<cv::Vec3b>();

    for (; itstart_1 != itend_1;  ++itstart, ++itstart_1, ++itstart_2) {
        for (size_t loop_i = 0; loop_i < image.channels(); loop_i++) {
            (*itstart)[loop_i] = (alpha * ((*itstart_1)[loop_i]) + (1 - alpha) * ((*itstart_2)[loop_i]));
        }
    }
    return cv::Mat(outputMat);
}

/* image sharp filter */
cv::Mat cImageFilter::imageSharp() {

    double sigma = 3;
    int threshold = 1;
    float amount = 50 / 100.0f;

    cv::Mat imgBlurred;
    cv::Mat outputMat(image.size(),image.type());
    GaussianBlur(image, imgBlurred, cv::Size(7,7), sigma, sigma);

    cv::Mat lowContrastMask = abs(image - imgBlurred) < threshold;
    outputMat = image * (1 + amount) + imgBlurred * (-amount);
    image.copyTo(outputMat, lowContrastMask);

    return cv::Mat(outputMat);
}

/* image spherize filter */
cv::Mat cImageFilter::spherizeWrap() {

    cv::Mat outputMat(image.size(),image.type());

    int width = image.cols;
    int height = image.rows;

    float R;
    float e;
    float a,b;
    float alpha = 0.75f;

    a = height / 2.0f; b = width / 2.0f;
    e = (float)width / (float)height;
    R = std::min(a,b);

    cv::Point Center(width / 2, height / 2);

    float radius,Dis,new_x, new_y;
    float p,q,x1,y1,x0,y0;
    float theta;

    cv::Mat_<cv::Vec3b>::iterator itend = image.end<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itstart = image.begin<cv::Vec3b>();
    cv::Mat_<cv::Vec3b>::iterator itOutput = image.begin<cv::Vec3b>();

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            y0 = Center.y-y;
            x0 = x-Center.x;
            Dis = x0*x0+y0*y0;
            if(Dis < R*R){
                theta = atan(y0 / (x0 + 0.00001f));
                if(x0 < 0)  theta = theta + DEGA_CONSTANT_PI;
                radius = asin(sqrt(Dis) / R)* R / (DEGA_CONSTANT_PI / 2);
                radius = (sqrt(Dis) - radius) * (1 - alpha) + radius;
                new_x = radius * cos(theta);
                new_y = radius * sin(theta);
                new_x = Center.x + new_x;
                new_y = Center.y - new_y;

                if(new_x < 0)   new_x = 0;
                if(new_x >= width - 1)  new_x = width - 2;
                if(new_y < 0)   new_y = 0;
                if(new_y >= height - 1) new_y = height - 2;

                x1 = (int)new_x;
                y1 = (int)new_y;

                p = new_x - x1;
                q = new_y - y1;

                for (int k = 0; k < image.channels(); k++){
                    outputMat.at<cv::Vec3b>(y, x)[k]=(1 - p) * (1 - q) * image.at<cv::Vec3b>(y1, x1)[k]+
                                                       (p) * (1 - q) * image.at<cv::Vec3b>(y1,x1 + 1)[k]+
                                                       (1-p) * (q) * image.at<cv::Vec3b>(y1 + 1,x1)[k]+
                                                       (p) * (q) * image.at<cv::Vec3b>(y1 + 1,x1 + 1)[k];
                }

            }

        }
    }

    return cv::Mat(outputMat);
}

}