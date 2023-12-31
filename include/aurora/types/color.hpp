#if !defined(COLOR_HPP)
#define COLOR_HPP

#include "aurora/utils/utils.hpp"

class Color
{
public:

	Color(float fr, float fg, float fb, float fa=1) : r(fr), g(fg), b(fb), a(fa) {};

	float r, g, b, a=1;

	static Color Red;

	vec4 ToVector()
	{
		return vec4{ r,g,b,a };
	}

	void operator=(const vec4& v) { r = v.x, g = v.y, b = v.z, a = v.w; }
	operator vec4()
	{
		return ToVector();
	}
};



#endif // COLOR_HPP
