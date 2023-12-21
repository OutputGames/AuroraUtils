#if !defined(COLOR_HPP)
#define COLOR_HPP

#include "aurora/utils/utils.hpp"

class Color
{
public:

	Color(float fr, float fg, float fb, float fa=1) : r(fr), g(fg), b(fb), a(fa) {};

	float r, g, b, a=1;

	static Color Red;
};



#endif // COLOR_HPP
