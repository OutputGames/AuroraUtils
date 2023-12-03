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

#include <glm/glm.hpp>

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
		ptr = std::make_shared<T>(t);
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


#endif // UTILS_HPP
