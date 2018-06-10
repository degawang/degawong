#pragma once

#include "string"
#include "opencv2/opencv.hpp"

namespace degawong {

	class cDegaGui {
	public:
		cDegaGui();
		cDegaGui(std::string _winName, std::string _trackBarName) {};
		virtual ~cDegaGui();

	public:
		inline
			void createWindow(const std::string _winName = "untitled window") const {
			cv::namedWindow(_winName);
		}

		inline
			void disImage(const cv::Mat _Mat, const std::string _winName = "untitled window")const {
			if (!_Mat.empty()) {
				cv::imshow(_winName, _Mat);
			}
		}

		template<typename _T = int>
		inline void createTrackBar(std::string _trackBarName, std::string _winName,
			int barCount, _T _minValue, _T _maxValue, cv::TrackbarCallback _callBackFunc) const {
			cv::setTrackbarMax(_trackBarName, _winName, _maxValue);
			cv::setTrackbarMin(_trackBarName, _winName, _minValue);
			cv::createTrackbar(_trackBarName, _winName, _minValue, 255, _callBackFunc);
		}

		template<typename _T = int>
		inline void createTrackBar(int barCount, _T _minValue, _T _maxValue, cv::TrackbarCallback _callBackFunc,
			std::string _trackBarName = "untitled track bar", std::string _winName = "untitled window") const {
			cv::setTrackbarMax(_trackBarName, _winName, _maxValue);
			cv::setTrackbarMin(_trackBarName, _winName, _minValue);
			cv::createTrackbar(_trackBarName, _winName, _minValue, 255, _callBackFunc);
		}

		template<typename _T = int>
		inline _T getTrackBarValue(std::string _trackBarName, std::string _winName) {
			return cv::getTrackbarPos(_trackBarName, _winName);
		}

		template<typename _T = int>
		inline _T setTrackBarValue(std::string _trackBarName, std::string _winName, _T _barValue) {
			return cv::setTrackbarPos(_trackBarName, _winName, _barValue);
		}

		inline
			void setNouseCallBack(std::string _winName, cv::Mat _Mat, cv::MouseCallback _callBackFunc) {
			setMouseCallback(_winName, _callBackFunc, &_Mat);
		}

	private:
		void defaultMouseCallBack(int event, int x, int y, int flags, void* userdata) {

		}

		void defaultTrackBarCallBack(int pos, void* userdata) {

		}

	};
}