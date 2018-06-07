#pragma once

#include "string"

namespace degawong {

class cDegaException {
public:
	cDegaException();
	cDegaException(std::string _exceReason)
		: exceReason(_exceReason) {};
	virtual ~cDegaException();

public:

	inline
		std::string getExceReason() const {
		return exceReason;
	}

private:
	std::string exceReason;
};

}

