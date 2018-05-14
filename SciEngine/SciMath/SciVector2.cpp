#include "SciVector2.h"
#include "SciMath.h"

// Operators
SciVector2 SciVector2::operator+(const SciVector2& other) const
{
	SciVector2 tmp;
	tmp.x = x + other.x;
	tmp.y = y + other.y;
	return tmp;
}

SciVector2& SciVector2::operator+=(const SciVector2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

SciVector2 SciVector2::operator-(const SciVector2& other) const
{
	SciVector2 tmp;
	tmp.x = x - other.x;
	tmp.y = y - other.y;
	return tmp;
}

SciVector2& SciVector2::operator-=(const SciVector2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

SciVector2 SciVector2::operator*(const float& scalar) const
{
	SciVector2 tmp;
	tmp.x = scalar * x;
	tmp.y = scalar * y;
	return tmp;
}

SciVector2& SciVector2::operator*=(const float& scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

SciVector2 SciVector2::operator/(const float& scalar) const
{
	float recip = 1.0f / scalar;
	return *this * recip;
}

SciVector2& SciVector2::operator/=(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this *= recip;
}

SciVector2 SciVector2::operator-() const
{
	SciVector2 tmp;
	tmp.x = -x;
	tmp.y = -y;
	return tmp;
}
// End of Operators

// Core operations
// Dot Product
float SciVector2::Dot(const SciVector2& other) const
{
	return x * other.x + y * other.y;
}

// Perpendicular Dot Product - analogous to Cross Product in 3D vectors
float SciVector2::PerpDot(const SciVector2& other) const
{
	return Perp().Dot(other);
}

// Magnitude (length) of vector squared
float SciVector2::MagSqr() const
{
	return x * x + y * y; // Dot Product of vector by itself
}

// Magnitude (length) of vector
float SciVector2::Mag() const
{
	return sciSqrt(x*x + y*y);
}

// Normalize vector
SciVector2& SciVector2::Normalize()
{
	const float mag_sqr = x * x + y * y;
	if(!mag_sqr)
	{
		return *this;
	}

	const float recip = 1.0f / sciSqrt(mag_sqr);
	x *= recip;
	y *= recip;
	return *this;
}

SciVector2 SciVector2::Normalized() const
{
	const float mag_sqr = x * x + y * y;
	if(!mag_sqr)
	{
		return *this;
	}

	SciVector2 tmp;
	const float recip = 1.0f / sciSqrt(mag_sqr);
	tmp.x = x * recip;
	tmp.y = y * recip;
	return tmp;
}

// Get counterclockwise perpendicular to this vector
SciVector2 SciVector2::Perp() const
{
	return SciVector2(-y, x);
}

// Eliminate microscopic values
void SciVector2::Clean()
{
	if(sciNearZero(x))
	{
		x = 0.0f;
	}
	if(sciNearZero(y))
	{
		y = 0.0f;
	}
}

// Vectors are almost equal
bool SciVector2::CloseTo(const SciVector2& other) const
{
	return sciAbs(x - other.x) <= 0.01f && sciAbs(y - other.y) <= 0.01f;
}
// End of Core operations

// Arithmetic operators functions
SciVector2 operator*(const float& scalar, const SciVector2& vector)
{
	return vector * scalar;
}
// End of Arithmetic operators functions

// Useful functions
float Distance(const SciVector2& point1, const SciVector2& point2)
{
	float x = point1.x - point2.x;
	float y = point1.y - point2.y;
	return sciSqrt(x*x + y*y);
}

float DistanceSqr(const SciVector2& point1, const SciVector2& point2)
{
	float x = point1.x - point2.x;
	float y = point1.y - point2.y;
	return x*x + y*y;
}
// End of Useful functions