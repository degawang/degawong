
#include "cRecursiveFind.h"

using namespace std;
using namespace degawong;

int main() 
{
	try{
		cRecursiveFind recuFind;
		recuFind.setDirName("e:\\image");
		auto imageList = recuFind.getImageNameList();
		for (int i = 0; i < 9; i++) {
			std::cout << imageList.size()<< std::endl;
		}
	}
	catch (...) {

	}

	return 0;
}