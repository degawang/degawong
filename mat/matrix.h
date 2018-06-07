#pragma once

namespace degawong {

template <typename _T>
class matrix
{
public:
	/* default constructor */
	matrix();
	/* copy constructor */
	matrix(const matrix& _matrix);
	/* custom-made constructor */
	matrix(int _row, int _col);

	virtual ~matrix();



};

}

