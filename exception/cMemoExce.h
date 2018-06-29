#pragma once
#include "cDegaException.h"
namespace degawong {

class cMemoExce :
	public cDegaException {
public:
	cMemoExce();
	cMemoExce(std::string _exceReason)
		: exceReason(_exceReason) {};
	~cMemoExce();

public:

	inline
		std::string what() const {
		return exceReason;
	}

private:
	std::string exceReason;
};

}

