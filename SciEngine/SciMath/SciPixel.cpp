#include "SciPixel.h"
#include "SciMath.h"

// Operators
SciPixel SciPixel::operator+(const SciPixel& other) const
{
	SciPixel tmp;
	tmp.r = r + other.r;
	tmp.g = g + other.g;
	tmp.b = b + other.b;
	tmp.a = a + other.a;

	return tmp;
}

SciPixel& SciPixel::operator+=(const SciPixel& other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;

	return *this;
}

SciPixel SciPixel::operator-(const SciPixel& other) const
{
	SciPixel tmp;
	tmp.r = r - other.r;
	tmp.g = g - other.g;
	tmp.b = b - other.b;
	tmp.a = a - other.a;

	return tmp;
}

SciPixel& SciPixel::operator-=(const SciPixel& other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	a -= other.a;

	return *this;
}

// Memberwise multiplication
SciPixel SciPixel::operator*(const SciPixel& other) const
{
	SciPixel tmp;
	tmp.r = r * other.r;
	tmp.g = g * other.g;
	tmp.b = b * other.b;
	tmp.a = a * other.a;
	
	return tmp;
}

// Memberwise multiplication
SciPixel& SciPixel::operator*=(const SciPixel& other)
{
	r *= other.r;
	g *= other.g;
	b *= other.b;
	a *= other.a;

	return *this;
}

SciPixel SciPixel::operator*(const float& scalar) const
{
	SciPixel tmp;
	tmp.r = scalar * r;
	tmp.g = scalar * g;
	tmp.b = scalar * b;
	tmp.a = scalar * a;

	return tmp;
}

SciPixel& SciPixel::operator*=(const float& scalar)
{
	r *= scalar;
	g *= scalar;
	b *= scalar;
	a *= scalar;

	return *this;
}

SciPixel SciPixel::operator/(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this * recip;
}

SciPixel& SciPixel::operator/=(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this *= recip;
}
// End of Operators

// Core operations
// Scale only color, but not alpha
SciPixel SciPixel::ScaleColor(float scalar) const
{
	SciPixel tmp;
	tmp.r = scalar * r;
	tmp.g = scalar * g;
	tmp.b = scalar * b;
	tmp.a = a;

	return tmp;
}

SciPixel& SciPixel::InvertAlpha()
{
	a = sciClamp(a);
	a = 1.0f - a;
	return *this;
}

// Returns correct, clamped alpha
float SciPixel::GetInverseAlpha() const
{
	float alpha = sciClamp(a);
	alpha = 1.0f - alpha;
	return alpha;
}
// End of Core operations

// Operator functions
SciPixel operator*(const float& scalar, const SciPixel& pixel)
{
	return pixel * scalar;
}
// End of Operator functions