#include "EvilSnake.h"

Route::Route()
{
	left = 0;
	right = 0;
	up = 0;
	down = 0;
}

Route& Route::operator=(int value)
{
	left = value;
	right = value;
	up = value;
	down = value;
	return *this;
}