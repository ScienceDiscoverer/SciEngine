#include "SciVector3.h"
#include "SciMath.h"

// Operators
SciVector3 SciVector3::operator+(const SciVector3& other) const
{
	SciVector3 tmp;
	tmp.x = x + other.x;
	tmp.y = y + other.y;
	tmp.z = z + other.z;
	return tmp;
}

SciVector3& SciVector3::operator+=(const SciVector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

SciVector3 SciVector3::operator-(const SciVector3& other) const
{
	SciVector3 tmp;
	tmp.x = x - other.x;
	tmp.y = y - other.y;
	tmp.z = z - other.z;
	return tmp;
}

SciVector3& SciVector3::operator-=(const SciVector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

SciVector3 SciVector3::operator*(const float& scalar) const
{
	SciVector3 tmp;
	tmp.x = scalar * x;
	tmp.y = scalar * y;
	tmp.z = scalar * z;
	return tmp;
}

SciVector3& SciVector3::operator*=(const float& scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

SciVector3 SciVector3::operator/(const float& scalar) const
{
	float recip = 1.0f / scalar;
	return *this * recip;
}

SciVector3& SciVector3::operator/=(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this *= recip;
}

SciVector3 SciVector3::operator-() const
{
	SciVector3 tmp;
	tmp.x = -x;
	tmp.y = -y;
	tmp.z = -z;
	return tmp;
}
// End of Operators

// Core operations
// Dot Product
float SciVector3::Dot(const SciVector3& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

// Cross Product
SciVector3 SciVector3::Cross(const SciVector3& other) const
{
	SciVector3 tmp;
	tmp.x = y * other.z - other.y * z;
	tmp.y = z * other.x - other.z * x;
	tmp.z = x * other.y - other.x * y;
	return tmp;
}

// Magnitude (length) of vector squared
float SciVector3::MagSqr() const
{
	return x * x + y * y + z * z; // Dot Product of vector by itself
}

// Magnitude (length) of vector
float SciVector3::Mag() const
{
	return sciSqrt(x*x + y*y + z*z);
}

// Normalize vector
SciVector3& SciVector3::Normalize()
{
	const float mag_sqr = x * x + y * y + z * z;
	if(!mag_sqr)
	{
		return *this;
	}
	
	float recip = 1.0f / sciSqrt(mag_sqr);
	x *= recip;
	y *= recip;
	z *= recip;
	return *this;
}

SciVector3 SciVector3::Normalized() const
{
	const float mag_sqr = x * x + y * y + z * z;
	if(!mag_sqr)
	{
		return *this;
	}

	SciVector3 tmp;
	const float recip = 1.0f / sciSqrt(mag_sqr);
	tmp.x = x * recip;
	tmp.y = y * recip;
	tmp.z = z * recip;
	return tmp;
}

// Eliminate microscopic values
void SciVector3::Clean()
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
}

// Vectors are almost equal
bool SciVector3::CloseTo(const SciVector3& other) const
{
	return sciAbs(x - other.x) <= 0.01f && sciAbs(y - other.y) <= 0.01f
		&& sciAbs(z - other.z) <= 0.01f;
}
// End of Core operations

// Arithmetic operators functions
SciVector3 operator*(const float& scalar, const SciVector3& vector)
{
	return vector * scalar;
}
// End of Arithmetic operators functions

// Relational operators functions
inline bool operator<(const SciVector3& lhs, const SciVector3& rhs)
{ 
	return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
}

inline bool operator==(const SciVector3& lhs, const SciVector3& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}
// End of Relational operators functions

// Useful functions
float Distance(const SciVector3& point1, const SciVector3& point2)
{
	float x = point1.x - point2.x;
	float y = point1.y - point2.y;
	float z = point1.z - point2.z;
	return sciSqrt(x*x + y*y + z*z);
}

float DistanceSqr(const SciVector3& point1, const SciVector3& point2)
{
	float x = point1.x - point2.x;
	float y = point1.y - point2.y;
	float z = point1.z - point2.z;
	return x*x + y*y + z*z;
}
// End of Useful functions