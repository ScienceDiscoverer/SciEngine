#include <cassert>
#include <cstring>
#include "SciMatrix2x2.h"
#include "SciMath.h"

// Constants
const size_t msize = sizeof(SciMatrix2x2);
const SciMatrix2x2 identity;

// Constructors
// Load Identity matrix
SciMatrix2x2::SciMatrix2x2()
{
	Identity();
}

// Construct from vectors (column-space) for post-mult. on column vector
SciMatrix2x2::SciMatrix2x2(const SciVector2& vec1, const SciVector2& vec2)
{
	SetColumns(vec1, vec2);
}
// End of Constructors

// Rule of 5
SciMatrix2x2::SciMatrix2x2(const SciMatrix2x2& other)
{
	memcpy(this, &other, msize);
}

SciMatrix2x2::SciMatrix2x2(SciMatrix2x2&& other)
{
	memcpy(this, &other, msize);
}

SciMatrix2x2& SciMatrix2x2::operator=(const SciMatrix2x2& other)
{
	memcpy(this, &other, msize);
	return *this;
}

SciMatrix2x2& SciMatrix2x2::operator=(SciMatrix2x2&& other)
{
	memcpy(this, &other, msize);
	return *this;
}
// End of Rule of 5

// Accsess functions
float& SciMatrix2x2::operator()(int row, int column)
{
	assert((row >= 0 && row < 2) && (column >= 0 && column < 2));
	return data[2*column + row]; // Column-major storage
}

const float& SciMatrix2x2::operator()(int row, int column) const
{
	assert((row >= 0 && row < 2) && (column >= 0 && column < 2));
	return data[2*column + row];
}

SciMatrix2x2& SciMatrix2x2::SetRows(const SciVector2& vec1, const SciVector2& vec2)
{
	data[0] = vec1.x;
	data[2] = vec1.y;
	data[1] = vec2.x;
	data[3] = vec2.y;

	return *this;
}

SciMatrix2x2& SciMatrix2x2::SetColumns(const SciVector2& vec1, const SciVector2& vec2)
{
	data[0] = vec1.x;
	data[1] = vec1.y;
	data[2] = vec2.x;
	data[3] = vec2.y;

	return *this;
}

SciVector2 SciMatrix2x2::GetRow(int row) const
{
	assert(row >= 0 && row < 2);

	switch(row)
	{
	case 0: return SciVector2(data[0], data[2]);
	case 1: return SciVector2(data[1], data[3]);
	default: return zero_vec2;
	}
}

SciVector2 SciMatrix2x2::GetColumn(int col) const
{
	assert(col >= 0 && col < 2);

	switch(col)
	{
	case 0: return SciVector2(data[0], data[1]);
	case 1: return SciVector2(data[2], data[3]);
	default: return zero_vec2;
	}
}
// End of Accsess functions

// Arithmetic operators
SciMatrix2x2 SciMatrix2x2::operator+(const SciMatrix2x2& other) const
{
	SciMatrix2x2 tmp;

	for(int i = 0; i < 4; ++i)
	{
		tmp.data[i] = data[i] + other.data[i];
	}

	return tmp;
}

SciMatrix2x2& SciMatrix2x2::operator+=(const SciMatrix2x2& other)
{
	for(int i = 0; i < 4; ++i)
	{
		data[i] += other.data[i];
	}

	return *this;
}

SciMatrix2x2 SciMatrix2x2::operator-(const SciMatrix2x2& other) const
{
	SciMatrix2x2 tmp;

	for(int i = 0; i < 4; ++i)
	{
		tmp.data[i] = data[i] - other.data[i];
	}

	return tmp;
}

SciMatrix2x2& SciMatrix2x2::operator-=(const SciMatrix2x2& other)
{
	for(int i = 0; i < 4; ++i)
	{
		data[i] -= other.data[i];
	}

	return *this;
}

SciMatrix2x2 SciMatrix2x2::operator*(const SciMatrix2x2& other) const
{
	SciMatrix2x2 tmp;

	tmp.data[0] = data[0]*other.data[0] + data[2]*other.data[1];
	tmp.data[2] = data[0]*other.data[2] + data[2]*other.data[3];
	tmp.data[1] = data[1]*other.data[0] + data[3]*other.data[1];
	tmp.data[3] = data[1]*other.data[2] + data[3]*other.data[3];

	return tmp;
}

SciMatrix2x2& SciMatrix2x2::operator*=(const SciMatrix2x2& other)
{
	SciMatrix2x2 tmp = *this * other;
	*this = tmp; // Does this copy an array? It should.
	return *this;
}

SciMatrix2x2 SciMatrix2x2::operator*(const float& scalar) const
{
	SciMatrix2x2 tmp;

	for(int i = 0; i < 4; ++i)
	{
		tmp.data[i] = data[i] * scalar;
	}

	return tmp;
}

SciMatrix2x2& SciMatrix2x2::operator*=(const float& scalar)
{
	for(int i = 0; i < 4; ++i)
	{
		data[i] *= scalar;
	}

	return *this;
}

// Post multiplication by column 2D vector
SciVector2 SciMatrix2x2::operator*(const SciVector2& vector) const
{
	SciVector2 tmp;
	tmp.x = data[0]*vector.x + data[2]*vector.y;
	tmp.y = data[1]*vector.x + data[3]*vector.y;
	return tmp;
}

SciMatrix2x2 SciMatrix2x2::operator/(const float& scalar) const
{
	float recip = 1.0f / scalar;
	return *this * recip;
}

SciMatrix2x2& SciMatrix2x2::operator/=(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this *= recip;
}
// End of Arithmetic Operators

// Relationsl operators
bool SciMatrix2x2::operator==(const SciMatrix2x2& other) const
{
	return !(bool)memcmp(this, &other, msize);
}

bool SciMatrix2x2::operator!=(const SciMatrix2x2& other) const
{
	return !(*this == other);
}
// End of Relationsl operators

// Core operations
SciMatrix2x2& SciMatrix2x2::Transpose()
{
	float tmp = data[2];
	data[2] = data[1];
	data[1] = tmp;
	return *this;
}

float SciMatrix2x2::Determinant() const
{
	return data[0]*data[3] - data[2]*data[1];
}

// Create count.clock. rotation matrix
SciMatrix2x2& SciMatrix2x2::CalcRotation(float angle)
{
	float cos_a, sin_a;
	sciCosSin(angle, cos_a, sin_a);
	data[0] = cos_a;
	data[1] = sin_a;
	data[2] = -sin_a;
	data[3] = cos_a;

	return *this;
}
// Make rotation matrix clockwise
SciMatrix2x2& SciMatrix2x2::MakeClock()
{
	data[1] = -sciAbs(data[1]);
	data[2] = sciAbs(data[2]);
	return *this;
}

// Make rotation matrix counterclockwise
SciMatrix2x2& SciMatrix2x2::MakeCountClock()
{
	data[1] = sciAbs(data[1]);
	data[2] = -sciAbs(data[2]);
	return *this;
}

// Reset matrix to Identity matrix
SciMatrix2x2& SciMatrix2x2::Identity()
{
	data[0] = 1.0f;
	data[3] = 1.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;

	return *this;
}

bool SciMatrix2x2::IsIdentity()
{
	return !(bool)memcmp(this, &identity, msize);
}
// End of Core operations

// Operator functions
SciMatrix2x2 operator*(const float& scalar, const SciMatrix2x2& matrix)
{
	return matrix * scalar;
}

// Its still post multiplication by column 2D vector
SciVector2 operator*(const SciVector2& vector, const SciMatrix2x2& matrix)
{
	return matrix * vector;
}
// End of Operator functions