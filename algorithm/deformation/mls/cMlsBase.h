#pragma once

#include "mat.h"
#include "string"
#include "iostream"


namespace degawong {

class cMlsBase {
public:
	cMlsBase();
	virtual ~cMlsBase();




private:
	int ctrlPointsNum;
	


private:
	void preCompute();
};

}

