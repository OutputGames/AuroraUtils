#if !defined(UTILS_HPP)
#define UTILS_HPP

#include <string>
#include <iostream>
#include  <vector>
#include <map>
#include <optional>
#include <set>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <fstream>
#include <sstream>
#include <chrono>
#include <utility>
#include <filesystem>
#include <memory>
#include <stdexcept>
#include <cstdlib>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>

#include "json.hpp"

using namespace nlohmann;

#include "SDL.h"

using namespace glm;

#define flt (float)
#define PI 3.141592653589793238462643383279502884197

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

#ifndef AURORA_DLLBUILD
#define AURORA_API __declspec(dllexport)
#else
#define AURORA_API __declspec(dllimport)
#endif

#define SIZE_OF(a) sizeof(a) / sizeof(decltype(a))


#define cast(O,T) static_cast<T>(O)

#define loop(c, cl, vn) alignas(sizeof(cl)) cl vn##c;

#define IS inline static

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifndef EPSILON
#define EPSILON 0.000001f
#endif

#ifndef DEG2RAD
#define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
#define RAD2DEG (180.0f/PI)
#endif

#ifndef JSON_INDENT_AMOUNT
#define JSON_INDENT_AMOUNT 2
#endif

template <typename T>

struct Ref
{

	Ref(nullptr_t) {}
	Ref() {}

	Ref(T t)
	{
		ptr = std::make_shared<T>(t);
	}

	Ref(T* t)
	{
		ptr = std::make_shared<T>(*t);
	}

	Ref(std::shared_ptr<T> t)
	{
		ptr = t;
	}

	T* operator->()
	{
		return ptr.get();
	}

	T* Get() { return ptr.get(); }

private:

	std::shared_ptr<T> ptr=nullptr;

};


struct ustring
{
	static bool hasEnding(std::string const& fullString, std::string const& ending) {
		if (fullString.length() >= ending.length()) {
			return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
		}
		else {
			return false;
		}
	}

	static std::string substring(std::string t, std::string s)
	{

		std::string ts = t;

		std::string::size_type i = ts.find(s);

		if (i != std::string::npos)
			ts.erase(i, s.length());
		return ts;
	}

	static std::string replace(std::string s, const std::string& from, const std::string& to)
	{

		std::string str = s;

		if (from.empty())
			return s;
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}

		return str;
	}
};

struct SDLUtils
{
	static void Init(u32 flags)
	{
		if (SDL_Init(flags) !=0)
		{
			std::cout << "Error " << SDL_GetError() << std::endl;
			exit(-1);
		}
	}
};

struct uvector
{
	// Function to print the 
	// index of an element
	template <typename T>
	static int GetIndexOfElement(std::vector<T> v, T K)
	{
		auto it = std::find(v.begin(), v.end(), K);

		// If element was found 
		if (it != v.end())
		{

			// calculating the index 
			// of K 
			int index = it - v.begin();
			return index;
		}
		else {
			return -1;
		}
	}


	template <typename T>
	static bool ContainsElement(std::vector<T> v, T K)
	{
		auto it = std::find(v.begin(), v.end(), K);

		// If element was found 
		if (it != v.end())
		{

			return true;
		}
		else {
			return false;
		}
	}
};



#endif // UTILS_HPP
