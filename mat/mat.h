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
class mat {

public:
	/* default constructor */
	mat() 
		: width(0), height(0), format(0), value(0), chanels(0), subRegion(0),
		memoExce("mat class: out of memory"), paraExce("mat class: invalid parameter"), fileExce("mat class: file open error") {		
		pitch[0] = 0;
		pitch[1] = 0;
		pitch[2] = 0;
		pitch[3] = 0;
		
		data[0] = nullptr;
		data[1] = nullptr;
		data[2] = nullptr;
		data[3] = nullptr;

		refCount = nullptr;
	};
	/* copy constructor */
	mat(const mat<_T> &_mat)
		: width(_mat.width), height(_mat.height), format(0), value(0), chanels(_mat.chanels), subRegion(0), 
		memoExce("mat class: out of memory"), paraExce("mat class: invalid parameter"), fileExce("mat class: file open error") {
		try {			

			data[0] = nullptr;
			data[1] = nullptr;
			data[2] = nullptr;
			data[3] = nullptr;
			
			refCount = nullptr;

			pitch[0] = _mat.pitch[0];
			pitch[1] = _mat.pitch[1];
			pitch[2] = _mat.pitch[2];
			pitch[3] = _mat.pitch[3];

			allocate();

			for (int loop_i = 0; loop_i < height; loop_i++) {
				for (int loop_j = 0; loop_j < width; loop_j++) {
					for (int loop_k = 0; loop_k < chanels; loop_k++) {
						data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] = _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
					}
				}
			}
		}
		catch (const cParaExce& exce) {
			std::cerr << exce.getExceReason() << std::endl;
			throw;
		}
	}
	/* move constructor */
	mat(mat<_T> &&_mat)
		: width(_mat.width), height(_mat.height), format(_mat.format), value(0), chanels(_mat.chanels), subRegion(_mat.subRegion),
		memoExce("mat class: out of memory"), paraExce("mat class: invalid parameter"), fileExce("mat class: file open error") {
		try {

			setData(_mat);
			clearData(_mat);

		}
		catch (const cParaExce& exce) {
			std::cerr << exce.getExceReason() << std::endl;
			throw;
		}
	}
	/* custom-made constructor */
	mat(int _width, int _height, int _chanels = 3, _T _value = 0)
		: width(_width), height(_height), format(0), value(_value), chanels(_chanels), subRegion(0), 
		memoExce("mat class: out of memory"), paraExce("mat class: invalid parameter"), fileExce("mat class: file open error") {
		pitch[0] = caculPitch();
		pitch[1] = 0;
		pitch[2] = 0;
		pitch[3] = 0;
		data[0] = nullptr;
		data[1] = nullptr;
		data[2] = nullptr;
		data[3] = nullptr;
		
		refCount = nullptr;

		allocate();
		*this = value;
	}
	/* custom-made constructor */
	mat(const cv::Mat &_mat)
		: width(_mat.cols), height(_mat.rows), format(0), value(0), chanels(_mat.channels()), subRegion(0),
		memoExce("mat class: out of memory"), paraExce("mat class: invalid parameter"), fileExce("mat class: file open error") {
		try {
			if (3 < _mat.channels()) {
				throw cParaExce("mat chanels should not bigger or equal than 3");
			}
			width = _mat.cols;
			height = _mat.rows;
			refCount = nullptr;

			pitch[0] = caculPitch();
			pitch[1] = 0;
			pitch[2] = 0;
			pitch[3] = 0;

			data[0] = nullptr;
			data[1] = nullptr;
			data[2] = nullptr;
			data[3] = nullptr;

			allocate();

			for (int loop_i = 0; loop_i < height; loop_i++) {
				for (int loop_j = 0; loop_j < width; loop_j++) {
					for (int loop_k = 0; loop_k < chanels; loop_k++) {
						data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] = _mat.at<cv::Vec3b>(loop_i, loop_j)[loop_k];
					}
				}
			}

		}
		catch (const cParaExce& exce) {
			std::cerr << exce.getExceReason() << std::endl;
		}
	}

	/* default destructor */
	virtual ~mat() { 
		if ((nullptr != data[0]) && (nullptr != refCount)) {
			if ((0 == subRegion) && (0 >= (--(*refCount)))) {
				delocate();
			}
		}
	}

public:	
	mat<_T>& operator ++ ();
	mat<_T>& operator -- ();
	mat<_T> operator ++ (int);
	mat<_T> operator -- (int);

	mat<_T>& operator = (const _T& _value);
	mat<_T> operator + (const _T& _value);
	mat<_T> operator - (const _T& _value);
	mat<_T> operator * (const _T& _value);
	mat<_T> operator / (const _T& _value);

	mat<_T>& operator = (mat<_T>& _mat);
	mat<_T>& operator = (mat<_T>&& _mat);	
	mat<_T> operator + (const mat<_T>& _mat);
	mat<_T> operator - (const mat<_T>& _mat);
	mat<_T> operator * (const mat<_T>& _mat);
	mat<_T> operator / (const mat<_T>& _mat);

	mat<_T>& operator += (const _T& _value);
	mat<_T>& operator -= (const _T& _value);
	mat<_T>& operator *=(const _T& _value);
	mat<_T>& operator /=(const _T& _value);

	mat<_T>& operator += (const mat<_T>& _mat);
	mat<_T>& operator -= (const mat<_T>& _mat);
	mat<_T>& operator *=(const mat<_T>& _mat);
	mat<_T>& operator /=(const mat<_T>& _mat);

	_T& operator()(const int& _width, const int& _height);

public:
	friend std::istream& operator >> (std::istream& is, mat<_T> &_mat) {
		is >> _mat.data[0];
		return is;
	}
	friend std::ostream& operator << (std::ostream& os, const mat<_T> &_mat) {
		try
		{
			//if (DEGA_FORMAT_BGR != _mat.format)
			//{
			//	throw cParaExce("invalid mat format");
			//}
			os << "degawong mat version _0_0_1" << std::endl;
			os << std::left << std::setw(10) << "mat width : " << _mat.width << std::endl;
			os << std::left << std::setw(10) << "mat height : " << _mat.height << std::endl;
			for (int loop_i = 0; loop_i < _mat.height; loop_i++) {
				for (int loop_j = 0; loop_j < _mat.width; loop_j++) {
					for (int loop_k = 0; loop_k < _mat.chanels; loop_k++) {
						os << std::right << std::setw(5) << _mat.data[0][loop_i * _mat.pitch[0] + loop_j * _mat.chanels + loop_k];
					}
				}
				os << std::endl;
			}
			return os;
		}
		catch (cParaExce &exce)
		{
			std::cerr << exce.getExceReason() << std::endl;
			throw;
		}
	}

public:
	/* mat memory allocator && delocate */;
	inline
	void allocate() {		
		try {

			refCount = new int(1);
			if (nullptr == refCount) {
				throw cMemoExce("out of memory");
			}
			data[0] = (_T*)fastMalloc(pitch[0] * height * sizeof(_T));
			if (nullptr == data[0]) {
				throw cMemoExce("out of memory");
			}
		}
		catch (const cMemoExce& exce) {
			std::cerr << exce.getExceReason() << std::endl;
			throw;
		}
	}
	inline
	void delocate() {
		width = 0;		
		height = 0;
		value = 0;
		chanels = 0;
		subRegion = false;
		delete refCount;
		fastFree(data[0]);
		pitch[0] = 0;
		pitch[1] = 0;
		pitch[2] = 0;
		pitch[3] = 0;
		data[0] = nullptr;
		data[1] = nullptr;
		data[2] = nullptr;
		data[3] = nullptr;
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
			std::cerr << exce.getExceReason() << std::endl;
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
	int getFormat() const;
	int getWidth() const;
	int getHeight() const;
	int getChanels() const;
	int getRefCount() const;
	int getPitch() const;
public:
	template<typename _IT>	
	inline _IT& at(int _width, int _height) {
		return data[0][_height * pitch[0] + _width * getChanels()];
	}
	inline
		_T* ptr(int _width, int _height) {
		return (_T*)(data[0] + _height * pitch[0] + _width * sizeof(_T));
	}
	inline
		_T* begin() {
		return (_T*)(&(data[0][0]));
	}
	inline
		_T* end() {
		return (_T*)(&(data[0][height * pitch[0]]));
	}
	inline
		mat<_T>& reshape(int _width, int _height) {

		width = _width;
		height = _height;
		pitch[0] = caculPitch();
		return *this;
	}

	inline
		mat<_T>& ref() {
		return *this;
	}
	inline
		mat<_T> clone() {
		return *this;
	}
public:
	mat<_T> imCorp(int _start_width, int _start_height, int _width, int _height);
public:	
	void print() const;
	void getVerson() const;
	void save(std::string pathName, unsigned char _Mode = std::ios::in | std::ios::trunc | std::ios::binary) const;
	void load(std::string pathName, unsigned char _Mode = std::ios::out | std::ios::trunc | std::ios::binary) const;
private:
	inline
		int caculPitch() {
		int bytes = 8 * getChanels();
		return ((long)(width) * (bytes) + 31) / 32 * 4;
	}
	inline
		void setData(const mat<_T>& _mat);
	inline
		void clearData(mat<_T>& _mat);
private:
	/* define multi  variables to fit some kinds of log request */
	//cMemoExce memoExce;
	//cParaExce paraExce;

private:
	/* define multi  variables to fit some kinds of exceptions */
	cMemoExce memoExce;
	cParaExce paraExce;
	cFileExce fileExce;
public:
	int *refCount;
private:
	int width;
	int height;
	int pitch[4];
	
	int format;	
	int chanels;

	bool subRegion;

	_T value;
	_T * data[4];
	
};

/* 
	function implementations 
*/

template<typename _T>
mat<_T>& mat<_T>::operator ++ () {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] += 1;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator -- () {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] -= 1;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator ++ (int) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		mat _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] += 1;
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator -- (int) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		mat _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] -= 1;
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator = (const _T& _value) {

	for (int loop_i = 0; loop_i < height; loop_i++) {
		for (int loop_j = 0; loop_j < width; loop_j++) {
			for (int loop_k = 0; loop_k < chanels; loop_k++) {
				data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] = _value;
			}
		}
	}

	return *this;
}

template<typename _T>
mat<_T> mat<_T>::operator + (const _T& _value) {
	try {

		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] = data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] + _value;
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce)
	{
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator - (const _T& _value) {
	try {
		//if ((chanels != _mat.chanels) ||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format)) {
		//	throw paraExce;
		//}

		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] = data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] - _value;
				}
			}
		}
		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator * (const _T& _value) {
	try {
		//if ((chanels != _mat.chanels) ||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format)) {
		//	throw paraExce;
		//}

		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] = data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] * _value;
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator / (const _T& _value) {
	try {
		if (0 == _value) {
			throw cParaExce("div zero error");
		}

		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] = data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] / _value;
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
inline mat<_T>& mat<_T>::operator = (mat<_T>& _mat) {
	if (this != &_mat) {
		
		(*_mat.refCount)++;

		if (nullptr != data[0]) {
			if (0 >= (--(*refCount))) {
				delocate();
			}
		}
		
		setData(_mat);
	}

	return *this;
}

template<typename _T>
inline mat<_T>& mat<_T>::operator = (mat<_T>&& _mat) {
	if (this != &_mat) {

		if (nullptr != data[0]) {
			if (0 >= (--(*refCount))) {
				delocate();
			}
		}

		setData(_mat);
		clearData(_mat);
	}

	return *this;
}

template<typename _T>
mat<_T> mat<_T>::operator + (const mat<_T>& _mat) {
	try {

		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}
		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] =
						data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] + _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce)
	{
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator - (const mat<_T>& _mat) {
	try {
		//if ((chanels != _mat.chanels) ||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format)) {
		//	throw paraExce;
		//}

		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] =
						data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] - _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator * (const mat<_T>& _mat) {
	try {
		//if ((chanels != _mat.chanels) ||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format)) {
		//	throw paraExce;
		//}

		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] =
						data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] * _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T> mat<_T>::operator / (const mat<_T>& _mat) {
	try {
		//if ((chanels != _mat.chanels) ||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format)) {
		//	throw paraExce;
		//}

		mat<_T> _out_mat(*this);

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {					
					_out_mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] =
						data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] / _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return _out_mat;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator += (const _T& _value) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] += _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator -= (const _T& _value) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] -= _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator *= (const _T& _value) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] *= _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator /= (const _T& _value) {
	try {
		if (0 == _value)
		{
			throw paraExce;
		}
		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] /= _value;
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator += (const mat<_T>& _mat) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] += _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator -= (const mat<_T>& _mat) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] -= _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator *= (const mat<_T>& _mat) {
	try {
		//if ((chanels != _mat.chanels)||
		//	(width != _mat.width) ||
		//	(height != _mat.height) ||
		//	(format != _mat.format) ||
		//	(DEGA_FORMAT_BGR != _mat.format))
		//{
		//	throw paraExce;
		//}

		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] *= _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
mat<_T>& mat<_T>::operator /= (const mat<_T>& _mat) {
	try {		
		for (int loop_i = 0; loop_i < height; loop_i++) {
			for (int loop_j = 0; loop_j < width; loop_j++) {
				for (int loop_k = 0; loop_k < chanels; loop_k++) {
					if (0 == _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k]) {
						throw cParaExce("divide zero error");
					}
					data[0][loop_i * pitch[0] + loop_j * chanels + loop_k] /= _mat.data[0][loop_i * pitch[0] + loop_j * chanels + loop_k];
				}
			}
		}

		return *this;
	}
	catch (const cMemoExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
inline _T & mat<_T>::operator()(const int & _width, const int & _height) {
	return data[0][_height * pitch[0] + _width * getChanels()];
}

/* get _mat format */
template<typename _T>
inline int mat<_T>::getFormat() const {
	return format;
}

/* get _mat width */
template<typename _T>
inline int mat<_T>::getWidth() const {
	return width;
}

/* get _mat height */
template<typename _T>
inline int mat<_T>::getHeight() const {
	return height;
}

/* get mat refer counts */
template<typename _T>
inline int mat<_T>::getRefCount() const {
	return refCount;
}

/* get mat pitch[0] length */
template<typename _T>
inline int mat<_T>::getPitch() const {
	return pitch[0];
}

template<typename _T>
inline int mat<_T>::getChanels() const {
	return chanels;
}

template<typename _T>
inline mat<_T> mat<_T>::imCorp(int _start_width, int _start_height, int _width, int _height) {
	mat<_T> _out_mat;
	_out_mat.width = _width;
	_out_mat.height = _height;
	_out_mat.chanels = chanels;
	_out_mat.format = format;
	_out_mat.subRegion = true;
	_out_mat.pitch[0] = pitch[0];
	_out_mat.pitch[1] = pitch[1];
	_out_mat.pitch[2] = pitch[2];
	_out_mat.pitch[3] = pitch[3];
	_out_mat.data[0] = &(data[0][_start_height * pitch[0] + _start_width * chanels]);
	_out_mat.data[1] = nullptr;
	_out_mat.data[2] = nullptr;
	_out_mat.data[3] = nullptr;

	return _out_mat;
}

template<typename _T>
void mat<_T>::print() const {
	std::cout << *this << std::endl;
}

template<typename _T>
void mat<_T>::save(std::string pathName, unsigned char _Mode) const {

	try {
		std::ofstream out(pathName, _Mode);
		if (!out.is_open()) {
			throw cFileExce("open file error");
		}
		out << *this << std::endl;
		out.close();
	}
	catch (cFileExce& exce) {
		std::cerr << exce.getExceReason() << std::endl;
		throw;
	}
}

template<typename _T>
inline void mat<_T>::setData(const mat<_T>& _mat) {

	width = _mat.width;
	height = _mat.height;
	format = _mat.format;
	chanels = _mat.chanels;
	refCount = _mat.refCount;
	subRegion = _mat.subRegion;

	pitch[0] = _mat.pitch[0];
	pitch[1] = _mat.pitch[1];
	pitch[2] = _mat.pitch[2];
	pitch[3] = _mat.pitch[3];

	data[0] = _mat.data[0];
	data[1] = _mat.data[1];
	data[2] = _mat.data[2];
	data[3] = _mat.data[3];
}

template<typename _T>
inline void mat<_T>::clearData(mat<_T>& _mat) {

	/* destroy old temp object */
	_mat.width = 0;
	_mat.height = 0;
	_mat.chanels = 0;
	_mat.format = 0;
	_mat.value = 0;

	_mat.pitch[0] = 0;
	_mat.pitch[1] = 0;
	_mat.pitch[2] = 0;
	_mat.pitch[3] = 0;

	_mat.data[0] = nullptr;
	_mat.data[1] = nullptr;
	_mat.data[2] = nullptr;
	_mat.data[3] = nullptr;

	_mat.refCount = nullptr;
}

template<typename _T>
void mat<_T>::getVerson() const {
	std::cout << "mat version _0_0_1" << std::endl;
}


}

