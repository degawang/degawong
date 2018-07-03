#pragma once

#include "matrix.h"

#ifndef __cplusplus
#  error matx.hpp header must be compiled as C++
#endif

namespace degawong {

template <typename _T, int _width, int _height, int _chanels = 1>
class matx {
public:
	matx() : width(_width), height(_height), chanels(_chanels) { 
		allocate(); 
	}
	~matx() {
		if ((nullptr != data) && (nullptr != refCount) && (0 >= (--(*refCount)))) {
			delocate();
		}
	}

public:
	/* matx memory allocator && delocate */;
	inline
		void allocate() {
		try {

			refCount = new int(1);
			if (nullptr == refCount) {
				throw cMemoExce("out of memory");
			}
			data = (_T*)fastMalloc(width * height * chanels * sizeof(_T));
			if (nullptr == data) {
				throw cMemoExce("out of memory");
			}
		}
		catch (const cMemoExce& exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}
	inline
		void delocate() {
		delete refCount;
		fastFree(data);
	}
	template<typename _IT> static
		inline	_IT* alignPtr(_IT* ptr, int n = (int)sizeof(_IT)) {
		return (_IT*)(((size_t)ptr + n - 1) & -n);
	}
	void* fastMalloc(size_t size) {
		try {

			_T* tempData = (_T*)malloc(size + sizeof(void*) + DEGA_ALLIGN_NUM);
			if (nullptr == tempData) {
				throw cMemoExce("out of memory");
			}
			_T** alligningData = alignPtr((_T**)tempData + 1, DEGA_ALLIGN_NUM);
			alligningData[-1] = tempData;
			return alligningData;
		}
		catch (const cMemoExce& exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}
	inline
		void fastFree(void* ptr) {
		if (ptr) {
			_T* alligningData = ((_T**)ptr)[-1];
			free(alligningData);
		}
	}

public:
	//friend _T operator [] (const matx<_T, width, height, 1> &_matx, int _height) {
	//	return _matx.data[_height * width + width - 1];
	//}
	//friend std::istream& operator >> (std::istream& is, matx &_matx) {
	//	is >> _matx.data[0];
	//	return is;
	//}
	//friend std::ostream& operator << (std::ostream& os, const matx<_T, width, height, 1> &_matx) {
	//	try {
	//		//if (DEGA_FORMAT_BGR != _mat.format)
	//		//{
	//		//	throw cParaExce("invalid mat format");
	//		//}
	//		os << "degawong matx version _0_0_1" << std::endl;
	//		os << std::left << std::setw(10) << "matx width : " << _matx.width << std::endl;
	//		os << std::left << std::setw(10) << "matx height : " << _matx.height << std::endl;
	//		for (int loop_i = 0; loop_i < _matx.height; loop_i++) {
	//			for (int loop_j = 0; loop_j < _matx.width; loop_j++) {
	//				for (int loop_k = 0; loop_k < _matx.chanels; loop_k++) {
	//					os << std::right << std::setw(5) << _matx.data[0][loop_i * _matx.width + loop_j * _matx.chanels + loop_k];
	//				}
	//			}
	//			os << std::endl;
	//		}
	//		return os;
	//	} catch (cParaExce &exce) {
	//		std::cerr << exce.what() << std::endl;
	//		throw;
	//	}
	//}
public:
	inline virtual _T& at(int _height, int _width = 1) {
		return data[_height * width + _width];
	}

public:
	int *refCount;
private:
	int width;
	int height;
	int chanels;

	_T value;
	_T * data;
};




}