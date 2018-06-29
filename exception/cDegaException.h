#pragma once

#include "string"
#include "iostream"

namespace degawong {

class cDegaException {
public:
	cDegaException();
	cDegaException(std::string _exceReason)
		: exceReason(_exceReason) {};
	virtual ~cDegaException();

public:

	inline
		std::string what() const {
		return exceReason;
	}

private:
	std::string exceReason;
};

}

