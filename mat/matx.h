
#include "matrix.h"

namespace degawong {

template <typename _T, int _width, int _height, int _chanels>
class matx : public matrix<float> {
public:
	matx() : width(_width), height(_height), chanels(_chanels) { 
		allocate(); 
	}
	~matx() {
		delocate();
		if ((nullptr != data) && (nullptr != refCount) && (0 >= (--(*refCount)))) {
			delocate();
		}
	}

public:
	/* matx memory allocator && delocate */;
	inline
		void allocate() {
		try
		{
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
			std::cerr << exce.getExceReason() << std::endl;
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
		try
		{
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
	int *refCount;
private:
	int width;
	int height;
	int chanels;

	_T value;
	_T * data;
};




}