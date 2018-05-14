#include "SciVector4.h"
#include "SciMath.h"

//Operators
SciVector4 SciVector4::operator+(const SciVector4& other) const
{
	SciVector4 tmp;
	tmp.x = x + other.x;
	tmp.y = y + other.y;
	tmp.z = z + other.z;
	tmp.w = w + other.w;
	return tmp;
}

SciVector4& SciVector4::operator+=(const SciVector4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
	return *this;
}

SciVector4 SciVector4::operator-(const SciVector4& other) const
{
	SciVector4 tmp;
	tmp.x = x - other.x;
	tmp.y = y - other.y;
	tmp.z = z - other.z;
	tmp.w = z - other.w;
	return tmp;
}

SciVector4& SciVector4::operator-=(const SciVector4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
	return *this;
}

SciVector4 SciVector4::operator*(const float& scalar) const
{
	SciVector4 tmp;
	tmp.x = scalar * x;
	tmp.y = scalar * y;
	tmp.z = scalar * z;
	tmp.w = scalar * w;
	return tmp;
}

SciVector4& SciVector4::operator*=(const float& scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

SciVector4 SciVector4::operator/(const float& scalar) const
{
	float recip = 1.0f / scalar;
	return *this * recip;
}

SciVector4& SciVector4::operator/=(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this *= recip;
}

SciVector4 SciVector4::operator-() const
{
	SciVector4 tmp;
	tmp.x = -x;
	tmp.y = -y;
	tmp.z = -z;
	tmp.w = -w;
	return tmp;
}
// End of Operators

// Core operations
// Dot Product
float SciVector4::Dot(const SciVector4& other) const
{
	return x * other.x + y * other.y + z * other.z + w * other.w;
}

// Magnitude (length) of vector squared
float SciVector4::MagSqr() const
{
	return x * x + y * y + z * z + w * w; // Dot Product of vector by itself
}

// Magnitude (length) of vector
float SciVector4::Mag() const
{
	return sciSqrt(x*x + y*y + z*z + w*w);
}

// Normalize vector
SciVector4& SciVector4::Normalize()
{
	const float mag_sqr = x * x + y * y + z * z + w * w;
	if(!mag_sqr)
	{
		return *this;
	}

	float recip = 1.0f / sciSqrt(mag_sqr);
	x *= recip;
	y *= recip;
	z *= recip;
	w *= recip;
	return *this;
}

SciVector4 SciVector4::Normalized() const
{
	const float mag_sqr = x * x + y * y + z * z + w * w;
	if(!mag_sqr)
	{
		return *this;
	}

	SciVector4 tmp;
	const float recip = 1.0f / sciSqrt(mag_sqr);
	tmp.x = x * recip;
	tmp.y = y * recip;
	tmp.z = z * recip;
	tmp.w = w * recip;
	return tmp;
}

// Eliminate microscopic values
void SciVector4::Clean()
{
	if(sciNearZero(x))
	{
		x = 0.0f;
	}
	if(sciNearZero(y))
	{
		y = 0.0f;
	}
	if(sciNearZero(z))
	{
		z = 0.0f;
	}
	if(sciNearZero(w))
	{
		w = 0.0f;
	}
}

// Vectors are almost equal
bool SciVector4::CloseTo(const SciVector4& other) const
{
	return sciAbs(x - other.x) <= 0.01f && sciAbs(y - other.y) <= 0.01f
		&& sciAbs(z - other.z) <= 0.01f && sciAbs(w - other.w) <= 0.01f;
}
// End of Core operations

// Arithmetic operators functions
SciVector4 operator*(const float& scalar, const SciVector4& vector)
{
	return vector * scalar;
}
// End of Arithmetic operators functions

// Relational operators functions
inline bool operator<(const SciVector4& lhs, const SciVector4& rhs)
{
	return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w < rhs.w;
}

inline bool operator==(const SciVector4& lhs, const SciVector4& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w;
}
// End of Relational operators functions