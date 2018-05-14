#ifndef SCIMATH_H
#define SCIMATH_H

#include <cmath>

// Constants
const float epsilon = 1.0e-6f;
const float pi = 3.1415926f;
const float half_pi = 1.5707963f;
const float one_radian = 180.0f / pi;
const float one_degree = pi / 180.0f;

// Macroses
#define RAD_DEG(a) (a * one_radian)
#define DEG_RAD(a) (a * one_degree)

// Wrappers
inline float sciSqrt(const float x)
{
	return sqrtf(x);
}

inline int sciFloorI(const float x)
{
	return (int)floorf(x);
}

inline float sciFloor(const float x)
{
	return floorf(x);
}

inline int sciCeilingI(const float x)
{
	return (int)ceilf(x);
}

inline int sciRoundI(const float x)
{
	return (int)roundf(x);
}

inline float sciAbs(const float x)
{
	return fabsf(x);
}

// Break float into fractional and integral components
inline float sciIntFrac(float x, float& int_part)
{
	return modff(x, &int_part);
}

//Trigonometric functions
inline float sciCos(const float angle)
{
	return cosf(angle);
}

inline float sciSin(const float angle)
{
	return sinf(angle);
}

inline void sciCosSin(const float angle, float& cos_a, float& sin_a)
{
	cos_a = cosf(angle);
	sin_a = sinf(angle);
}

inline float sciArcCos(const float cosine)
{
	return acosf(cosine);
}

// Float specific operations
inline bool sciNearZero(const float x)
{
	return (sciAbs(x) <= epsilon);
}

inline void sciCleanF(float& x)
{
	if(sciNearZero(x))
	{
		x = 0.0f;
	}
}

// Usefull functions
inline float sciMax(const float x, const float y)
{
	return x < y ? y : x;
}

inline float sciMin(const float x, const float y)
{
	return x < y ? x : y;
}

inline float sciClamp(const float x)
{
	return sciMax(0.0f, sciMin(1.0f, x));
}

#endif /* SCIMATH_H */
