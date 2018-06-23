#pragma once

#include "string"
#include "opencv2/opencv.hpp"

namespace degawong {

	class cDegaGui {
	public:
		cDegaGui(): barPosition(0), leftButtonDownFlag(false) {}
		// cDegaGui(std::string _winName, std::string _trackBarName) : barPosition(0), leftButtonDownFlag(false) {};
		virtual ~cDegaGui();

	public:
		inline
			void createWindow(const std::string _winName = "untitled window") const {
			cv::namedWindow(_winName, CV_WINDOW_NORMAL);
		}

		inline
			void disImage(const cv::Mat _image, const std::string _winName = "untitled window")const {
			if (!_image.empty()) {
				cv::Mat image(900, 600, _image.type());
				cv::resize(_image, image, image.size(), cv::INTER_CUBIC);
				cv::namedWindow(_winName, CV_WINDOW_NORMAL);
				cv::imshow(_winName, image);
			}
		}

		template<typename _T = int>
		inline void createTrackBar(int barCount, _T _minValue, _T _maxValue, cv::TrackbarCallback _callBackFunc,
			std::string _trackBarName = "untitled track bar", std::string _winName = "untitled window", void* inputPara = 0)
		{
			cv::setTrackbarMax(_trackBarName, _winName, _maxValue);
			cv::setTrackbarMin(_trackBarName, _winName, _minValue);
			cv::createTrackbar(_trackBarName, _winName, &barPosition, barCount, _callBackFunc, inputPara);
		}

		template<typename _T = int>
		inline _T getTrackBarValue(std::string _trackBarName, std::string _winName) {
			return cv::getTrackbarPos(_trackBarName, _winName);
		}

		template<typename _T = int>
		inline _T setTrackBarValue(std::string _trackBarName, std::string _winName, _T _barPosition) {
			return cv::setTrackbarPos(_trackBarName, _winName, _barPosition);
		}

		inline
			void setNouseCallBack(std::string _winName, cv::Mat _image, cv::MouseCallback _callBackFunc) {
			//if (nullptr == _callBackFunc) {
			//	cv::setMouseCallback(_winName, defaultMouseCallBack, &_image);
			//}
			//else {
			//	cv::setMouseCallback(_winName, _callBackFunc, &_image);
			//}
			cv::setMouseCallback(_winName, _callBackFunc, &_image);
		}

	private:
		void defaultTrackBarCallBack(int barPosition, void* paraList) {
			int *para = (int*)paraList;
			*para = barPosition;
			cv::Mat image(512, 512, CV_8UC3);
			image = *para;
			cv::imshow("i am a good boy", image);
		}

		void defaultMouseCallBack(int eventHandle, int location_x, int location_y, int direction, void* inputPara) {
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
				case CV_EVENT_MBUTTONDOWN:
				{
					/* mid wheel down */
				}break;
				case CV_EVENT_MOUSEWHEEL:
				{
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
			}
		}

	private:
		int barPosition;
		cv::Rect rectToDraw;
		bool leftButtonDownFlag;
		//bool leftButtonDownFlag = false;

	};
}