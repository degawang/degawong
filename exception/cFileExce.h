#pragma once
#include "cDegaException.h"

namespace degawong {

class cFileExce :
	public cDegaException {
public:
	cFileExce();
	cFileExce(std::string _fileReason)
		: exceReason(_fileReason) {};
	~cFileExce();

public:
	friend std::istream& operator >> (std::istream& is, cFileExce &exce) {
		is >> exce.exceReason;
		return is;
	};
	friend std::ostream& operator << (std::ostream& os, const cFileExce &exce) {
		os << exce.what() << std::endl;
		return os;
	};
public:
	inline
		std::string what() const {
		return exceReason;
	}

private:
	std::string exceReason;

};
}

