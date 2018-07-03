#pragma once

#include "matx.h"

#ifndef __cplusplus
#  error matx.hpp header must be compiled as C++
#endif

namespace degawong {

template <typename _T, int _height> 
class vecRow : public matx<_T, 1, _height> {
public:
	vecRow() : width(1), height(_height), chanels(1) {
		allocate();
	}
	~vecRow() {
		delocate();
		if ((nullptr != data) && (nullptr != refCount) && (0 >= (--(*refCount)))) {
			delocate();
		}
	}

public:
	/* vec memory allocator && delocate */;
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
		} catch (const cMemoExce& exce) {
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
		} catch (const cMemoExce& exce) {
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
	int *refCount;
private:
	int width;
	int height;
	int chanels;

	_T value;
	_T * data;
};

template <typename _T, int _width> 
class vecCol : public matx<_T, _width, 1> {
public:
	vecCol() : width(_width), height(1), chanels(1) {
		allocate(); 
	}
	/* for mat.at<vec3x>[...] */
	vecCol(_T &_firstValue) : width(_width), height(1), chanels(1) {
		refCount = nullptr;
		data = &_firstValue;
	}
	~vecCol() {
		if ((nullptr != data) && (nullptr != refCount) && (0 >= (--(*refCount)))) {
			delocate();
		}
	}

public:
	/* vec memory allocator && delocate */;
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
	friend std::istream& operator >> (std::istream& is, vecCol & _vecCol) {
		is >> _vecCol.data[0];
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const vecCol & _vecCol) {
		try {
			os << "degawong vec version _0_0_1" << std::endl;
			os << std::left << std::setw(10) << "vec width : " << _vecCol.width << std::endl;
			os << std::left << std::setw(10) << "vec height : " << _vecCol.height << std::endl;
			for (int loop_i = 0; loop_i < _vecCol.height; loop_i++) {
				for (int loop_j = 0; loop_j < _vecCol.width; loop_j++) {
					for (int loop_k = 0; loop_k < _vecCol.chanels; loop_k++) {
						os << std::right << std::setw(5) << _vecCol.data[loop_i * _vecCol.width + loop_j * _vecCol.chanels + loop_k];
					}
				}
				os << std::endl;
			}
			return os;
		} catch (cParaExce &exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}

public:
	inline _T& operator [] (int _width) {
		return data[ _width];
	}
public:
	virtual _T& at(int _width, int _height = 1) {
		return data[(_height - 1) * width + _width];
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

using vec3i = vecCol<int, 3>;
using vec3f = vecCol<float, 3>;
using vec3c = vecCol<unsigned char, 3>;

using vec4i = vecCol<int, 4>;
using vec4f = vecCol<float, 4>;
using vec4c = vecCol<unsigned char, 4>;

}