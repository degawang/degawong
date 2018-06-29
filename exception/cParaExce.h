#pragma once
#include "cDegaException.h"
namespace degawong {

class cParaExce :
	public cDegaException {
public:
	cParaExce();
	cParaExce(std::string _exceReason)
		: exceReason(_exceReason) {};
	~cParaExce();

public:
	friend std::istream& operator >> (std::istream& is, cParaExce &exce) {
		is >> exce.exceReason;
		return is;
	};
	friend std::ostream& operator << (std::ostream& os, const cParaExce &exce) {
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

