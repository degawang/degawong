#pragma once

#include "fstream"
#include "iomanip"
#include "iostream"
#include "cFileExce.h"
#include "cParaExce.h"
#include "cMemoExce.h"
#include "degaFormat.h"
#include "degaPredefine.h"

#include "opencv2/opencv.hpp"

namespace degawong {

template <typename _T>
class matrix
{
public:
	/* default constructor */
	matrix()
		: width(0), pitch(0), height(0), value(0), chanels(0), subRegion(0) {

		data = nullptr;
		refCount = nullptr;
	}
	/* copy constructor */
	matrix(const matrix& _matrix)
		: width(_matrix.width), pitch(_matrix.width * _matrix.chanels), height(_matrix.height), value(0), chanels(_matrix.chanels), subRegion(0) {
		try {

			data = nullptr;			
			refCount = nullptr;

			allocate();

			for (int loop_i = 0; loop_i < height; loop_i++) {
				for (int loop_j = 0; loop_j < width; loop_j++) {
					for (int loop_k = 0; loop_k < chanels; loop_k++) {
						data[loop_i * pitch + loop_j * chanels + loop_k] = _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
					}
				}
			}
		}
		catch (const cParaExce& exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}
	/* move constructor */
	matrix(matrix&& _matrix)
		: width(_matrix.width), pitch(_matrix.width * _matrix.chanels), height(_matrix.height), value(0), chanels(_matrix.chanels), subRegion(_matrix.subRegion) {
		try {
			setData(_matrix);
			clearData(_matrix);
		}
		catch (const cParaExce& exce) {
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}
	/* custom-made constructor */
	matrix(int _height, int _width, int _chanels)
		: width(_width), pitch(_width * _chanels), height(_height), value(0), chanels(_chanels), subRegion(0) {
		data = nullptr;
		refCount = nullptr;
		allocate();

		*this = 8;
	}

	/* custom-made constructor */
	matrix(const cv::Mat &_matrix)
		: width(_matrix.cols), pitch(_matrix.cols), height(_matrix.rows), value(0), chanels(_matrix.channels()), subRegion(0) {
		try {
			if (3 < _matrix.channels()) {
				throw cParaExce("_matrix chanels should not bigger or equal than 3");
			}
			width = _matrix.cols;
			height = _matrix.rows;
			refCount = nullptr;
			data = nullptr;
			allocate();

			for (int loop_i = 0; loop_i < height; loop_i++) {
				for (int loop_j = 0; loop_j < width; loop_j++) {
					for (int loop_k = 0; loop_k < chanels; loop_k++) {
						data[loop_i * pitch + loop_j * chanels + loop_k] = _matrix.at<cv::Vec3b>(loop_i, loop_j)[loop_k];
					}
				}
			}

		}
		catch (const cParaExce& exce) {
			std::cerr << exce.what() << std::endl;
		}
	}

	virtual ~matrix() {
		if ((nullptr != data) && (nullptr != refCount)) {
			if ((0 == subRegion) && (0 >= (--(*refCount)))) {
				delocate();
			}
		}
	}

public:
	friend std::istream& operator >> (std::istream& is, matrix<_T> &_matrix) {
		is >> _matrix.data;
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const matrix<_T> &_matrix) {
		try {

			os << "degawong matrix version _0_0_1" << std::endl;
			os << std::left << std::setw(10) << "matrix width : " << _matrix.width << std::endl;
			os << std::left << std::setw(10) << "matrix height : " << _matrix.height << std::endl;
			for (int loop_i = 0; loop_i < _matrix.height; loop_i++) {
				for (int loop_j = 0; loop_j < _matrix.width; loop_j++) {
					for (int loop_k = 0; loop_k < _matrix.chanels; loop_k++) {
						os << std::right << std::setw(5) << _matrix.data[loop_i * _matrix.pitch + loop_j * _matrix.chanels + loop_k];
					}
				}
				os << std::endl;
			}
			return os;
		}
		catch (cParaExce &exce)
		{
			std::cerr << exce.what() << std::endl;
			throw;
		}
	}

public:
	matrix<_T>& operator ++ ();
	matrix<_T>& operator -- ();
	matrix<_T> operator ++ (int);
	matrix<_T> operator -- (int);

	matrix<_T>& operator = (const _T& _value);
	matrix<_T> operator + (const _T& _value);
	matrix<_T> operator - (const _T& _value);
	matrix<_T> operator * (const _T& _value);
	matrix<_T> operator / (const _T& _value);

	matrix<_T>& operator = (matrix<_T>& _mat);
	matrix<_T>& operator = (matrix<_T>&& _mat);
	matrix<_T> operator + (const matrix<_T>& _matrix);
	matrix<_T> operator - (const matrix<_T>& _matrix);
	matrix<_T> operator * (const matrix<_T>& _matrix);
	matrix<_T> operator / (const matrix<_T>& _matrix);

	matrix<_T>& operator += (const _T& _value);
	matrix<_T>& operator -= (const _T& _value);
	matrix<_T>& operator *=(const _T& _value);
	matrix<_T>& operator /=(const _T& _value);

	matrix<_T>& operator += (const matrix<_T>& _matrix);
	matrix<_T>& operator -= (const matrix<_T>& _matrix);
	matrix<_T>& operator *=(const matrix<_T>& _matrix);
	matrix<_T>& operator /=(const matrix<_T>& _matrix);

	_T& operator()(const int& _width, const int& _height);

public:
	/* matrix memory allocator && delocate */;
	inline
		void allocate() {
		try {

			refCount = new int(1);
			if (nullptr == refCount) {
				throw cMemoExce("out of memory");
			}
			data = (_T*)fastMalloc(pitch * height * sizeof(_T));
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
		width = 0;
		pitch = 0;
		height = 0;
		value = 0;
		chanels = 0;
		subRegion = false;
		delete refCount;
		fastFree(data);
		data = nullptr;
		refCount = nullptr;		
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
	inline
		matrix<_T>& reshape(int _width, int _height) {

		width = _width;
		height = _height;
		pitch = _width * chanels;
		return *this;
	}

public:
	matrix<_T> mul(const matrix<_T>& _matrix);
	matrix<_T> corp(int _start_row, int _start_col, int _width, int _height);

public:
	void print() const;
	void getVerson() const;
	void save(std::string pathName, unsigned char _Mode = std::ios::in | std::ios::trunc | std::ios::binary) const;
	void load(std::string pathName, unsigned char _Mode = std::ios::out | std::ios::trunc | std::ios::binary) const;
private:
	inline
		void setData(const matrix<_T>& _matrix);
	inline
		void clearData(matrix<_T>& _matrix);

public:
	int *refCount;
private:
	int width;
	int pitch;
	int height;	
	int chanels;

	_T value;
	_T * data;

	bool subRegion;
};

template<typename _T>
matrix<_T>& matrix<_T>::operator ++ () {
	try {

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] += 1;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator -- () {
	try {

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] -= 1;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator ++ (int) {
	try {

		matrix _out_matrix(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] += 1;
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator -- (int) {
	try {

		matrix _out_matrix(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] -= 1;
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator = (const _T& _value) {

	for (int loop_i = 0; loop_i < height; loop_i++) {
		for (int loop_j = 0; loop_j < width; loop_j++) {
			for (int loop_k = 0; loop_k < chanels; loop_k++) {
				data[loop_i * pitch + loop_j * chanels + loop_k] = _value;
			}
		}
	}

	return *this;
}

template<typename _T>
matrix<_T> matrix<_T>::operator + (const _T& _value) {
	try {
		matrix<_T> _out_matrix(width, height, chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * pitch + loop_j * chanels + loop_k] = data[loop_i * pitch + loop_j * chanels + loop_k] + _value;
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce)
	{
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator - (const _T& _value) {
	try {

		matrix<_T> _out_matrix(width, height, chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * pitch + loop_j * chanels + loop_k] = data[0][loop_i * pitch + loop_j * chanels + loop_k] - _value;
				}
			}
		}
		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator * (const _T& _value) {
	try {

		matrix<_T> _out_matrix(width, height, chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * pitch + loop_j * chanels + loop_k] = data[loop_i * pitch + loop_j * chanels + loop_k] * _value;
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator / (const _T& _value) {
	try {
		if (0 == _value) {
			throw cParaExce("div zero error");
		}

		matrix<_T> _out_matrix(width, height, chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * pitch + loop_j * chanels + loop_k] = data[loop_i * pitch + loop_j * chanels + loop_k] / _value;
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator = (matrix<_T>& _matrix) {
	if (this != &_matrix) {

		(*_matrix.refCount)++;

		if (nullptr != data) {
			if (0 >= (--(*refCount))) {
				delocate();
			}
		}

		setData(_matrix);
	}

	return *this;
}

template<typename _T>
matrix<_T>& matrix<_T>::operator = (matrix<_T>&& _matrix) {
	if (this != &_matrix) {

		if (nullptr != data) {
			if (0 >= (--(*refCount))) {
				delocate();
			}
		}

		setData(_matrix);
		clearData(_matrix);
	}

	return *this;
}

template<typename _T>
matrix<_T> matrix<_T>::operator + (const matrix<_T>& _matrix) {
	try {

		matrix<_T> _out_matrix(_matrix.width, _matrix.height, _matrix.chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * width * chanels + loop_j * chanels + loop_k] =
						data[loop_i * pitch + loop_j * chanels + loop_k] + _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce)
	{
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator - (const matrix<_T>& _matrix) {
	try {

		matrix<_T> _out_matrix(_matrix.width, _matrix.height, _matrix.chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * width * chanels + loop_j * chanels + loop_k] =
						data[loop_i * pitch + loop_j * chanels + loop_k] - _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator * (const matrix<_T>& _matrix) {
	try {
		matrix<_T> _out_matrix(_matrix.width, _matrix.height, _matrix.chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * width * chanels + loop_j * chanels + loop_k] =
						data[loop_i * pitch + loop_j * chanels + loop_k] * _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T> matrix<_T>::operator / (const matrix<_T>& _matrix) {
	try {

		matrix<_T> _out_matrix(_matrix.width, _matrix.height, _matrix.chanels);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_matrix.data[loop_i * width * chanels + loop_j * chanels + loop_k] =
						data[loop_i * pitch + loop_j * chanels + loop_k] / _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_matrix;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator += (const _T& _value) {
	try {

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] += _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator -= (const _T& _value) {
	try {

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] -= _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator *= (const _T& _value) {
	try {

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] *= _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator /= (const _T& _value) {
	try {
		if (0 == _value) {

			throw cParaExce("divide zero error");
		}
		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] /= _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator += (const matrix<_T>& _matrix) {
	try {
		if ((chanels != _matrix.chanels) ||
			(width != _matrix.width) ||
			(height != _matrix.height)) {
			throw cParaExce("unmatched matrix");
		}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] += _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator -= (const matrix<_T>& _matrix) {
	try {
		if ((chanels != _matrix.chanels) ||
			(width != _matrix.width) ||
			(height != _matrix.height)) {
			throw cParaExce("unmatched matrix");
		}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] -= _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator *= (const matrix<_T>& _matrix) {
	try {
		if ((chanels != _matrix.chanels) ||
			(width != _matrix.width) ||
			(height != _matrix.height)) {
			throw cParaExce("unmatched matrix");
		}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[loop_i * pitch + loop_j * chanels + loop_k] *= _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
matrix<_T>& matrix<_T>::operator /= (const matrix<_T>& _matrix) {
	try {
		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					if (0 == _matrix.data[loop_i * width * chanels + loop_j * chanels + loop_k]) {
						throw cParaExce("divide zero error");
					}
					data[loop_i * pitch + loop_j * chanels + loop_k] /= _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
_T & matrix<_T>::operator()(const int & _width, const int & _height) {
	return data[_height * pitch + _width * chanels];
}

template<typename _T>
inline matrix<_T> matrix<_T>::mul(const matrix<_T>& _matrix) {

	matrix<_T> _out_matrix(_matrix.width, _matrix.height, _matrix.chanels);

	for (int loop_i = 0; loop_i < height; loop_i++) {
		for (int loop_j = 0; loop_j < width; loop_j++) {
			for (int loop_k = 0; loop_k < chanels; loop_k++) {
				_out_matrix.data[loop_i * width * chanels + loop_j * chanels + loop_k] =
					data[loop_i * pitch + loop_j * chanels + loop_k] * _matrix.data[loop_i * pitch + loop_j * chanels + loop_k];
			}
		}
	}

	return matrix<_T>(_out_matrix);
}

template<typename _T>
inline matrix<_T> matrix<_T>::corp(int _start_height, int _start_width, int _width, int _height) {
	matrix<_T> _out_matrix;
	_out_matrix.width = _width;
	_out_matrix.height = _height;
	_out_matrix.chanels = chanels;
	_out_matrix.subRegion = true;
	_out_matrix.pitch = pitch;
	_out_matrix.data = &(data[_start_height * pitch + _start_width * chanels]);
	return matrix<_T>(_out_matrix);
}

template<typename _T>
void matrix<_T>::print() const {
	std::cout << *this << std::endl;
}

template<typename _T>
void matrix<_T>::save(std::string pathName, unsigned char _Mode) const {

	try {
		std::ofstream out(pathName, _Mode);
		if (!out.is_open()) {
			throw cFileExce("open file error");
		}
		out << *this << std::endl;
		out.close();
	}
	catch (cFileExce& exce) {
		std::cerr << exce.what() << std::endl;
		throw;
	}
}

template<typename _T>
inline void matrix<_T>::setData(const matrix<_T>& _matrix) {
	width = _matrix.width;
	pitch = _matrix.pitch;
	height = _matrix.height;
	chanels = _matrix.chanels;	
	refCount = _matrix.refCount;
	subRegion = _matrix.subRegion;
	data = _matrix.data;
}

template<typename _T>
inline void matrix<_T>::clearData(matrix<_T>& _matrix) {

	/* destroy old temp object */
	/* it is dangerous and should only been used in initial location*/
	_matrix.width = 0;
	_matrix.pitch = 0;
	_matrix.height = 0;
	_matrix.chanels = 0;
	_matrix.value = 0;
	_matrix.data = nullptr;
	_matrix.refCount = nullptr;
}

template<typename _T>
void matrix<_T>::getVerson() const {
	std::cout << "matrix version _0_0_1" << std::endl;
}

}

