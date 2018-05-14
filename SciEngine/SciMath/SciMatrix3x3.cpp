#include <cassert>
#include <cstring>
#include "SciMatrix3x3.h"

// Constants
const size_t msize = sizeof(SciMatrix3x3);
const SciMatrix3x3 identity;

// Helper function
void swap(float& x, float& y)
{
	float tmp = x;
	x = y;
	y = tmp;
}

// Constructors
// Load Identity matrix
SciMatrix3x3::SciMatrix3x3()
{
	Identity();
}

// Construct from vectors (column-space) for post-mult. on column vector
SciMatrix3x3::SciMatrix3x3(const SciVector3& vec1, const SciVector3& vec2, const SciVector3& vec3)
{
	SetColumns(vec1, vec2, vec3);
}

// Construct from 2x2 matrix
SciMatrix3x3::SciMatrix3x3(const SciMatrix2x2& matrix)
{
	SetRotation(matrix);
	data[8] = 1.0f;
	data[2] = 0.0f;
	data[5] = 0.0f;
	data[6] = 0.0f;
	data[7] = 0.0f;
}

// Construct affine transformation matrix from 2x2 rotation, and 2D translation vector
SciMatrix3x3::SciMatrix3x3(const SciMatrix2x2& rot, const SciVector2& trans)
{
	SetRotation(rot);
	SetTranslation(trans);
	data[2] = 0.0f;
	data[5] = 0.0f;
}
// End of Constructors

// Rule of 5
SciMatrix3x3::SciMatrix3x3(const SciMatrix3x3& other)
{
	memcpy(this, &other, msize);
}

SciMatrix3x3::SciMatrix3x3(SciMatrix3x3&& other)
{
	memcpy(this, &other, msize);
}

SciMatrix3x3& SciMatrix3x3::operator=(const SciMatrix3x3& other)
{
	memcpy(this, &other, msize);
	return *this;
}

SciMatrix3x3& SciMatrix3x3::operator=(SciMatrix3x3&& other)
{
	memcpy(this, &other, msize);
	return *this;
}
// End of Rule of 5

// Accsess functions
float& SciMatrix3x3::operator()(int row, int column)
{
	assert((row >= 0 && row < 3) && (column >= 0 && column < 3));
	return data[3*column + row]; // Column-major storage
}

const float& SciMatrix3x3::operator()(int row, int column) const
{
	assert((row >= 0 && row < 3) && (column >= 0 && column < 3));
	return data[3*column + row];
}

SciMatrix3x3& SciMatrix3x3::SetRows(const SciVector3& vec1, const SciVector3& vec2, const SciVector3& vec3)
{
	data[0] = vec1.x;
	data[3] = vec1.y;
	data[6] = vec1.z;
	data[1] = vec2.x;
	data[4] = vec2.y;
	data[7] = vec2.z;
	data[2] = vec3.x;
	data[5] = vec3.y;
	data[8] = vec3.z;

	return *this;
}

SciMatrix3x3& SciMatrix3x3::SetColumns(const SciVector3& vec1, const SciVector3& vec2, const SciVector3& vec3)
{
	data[0] = vec1.x;
	data[1] = vec1.y;
	data[2] = vec1.z;
	data[3] = vec2.x;
	data[4] = vec2.y;
	data[5] = vec2.z;
	data[6] = vec3.x;
	data[7] = vec3.y;
	data[8] = vec3.z;

	return *this;
}

// Set top left rotation block-matrix to matrix
SciMatrix3x3& SciMatrix3x3::SetRotation(const SciMatrix2x2& matrix)
{
	data[0] = matrix(0, 0);
	data[1] = matrix(1, 0);
	data[3] = matrix(0, 1);
	data[4] = matrix(1, 1);

	return *this;
}

// Set translation vector in affine matrix
SciMatrix3x3& SciMatrix3x3::SetTranslation(const SciVector2& vector)
{
	data[6] = vector.x;
	data[7] = vector.y;
	data[8] = 1.0f;
	return *this;
}

// Get top left rotation block-matrix from affine matrix
SciMatrix2x2 SciMatrix3x3::GetRotation() const
{
	SciVector2 x(data[0], data[1]);
	SciVector2 y(data[3], data[4]);
	return SciMatrix2x2(x, y);
}

// Get translation vector from affine matrix
SciVector2 SciMatrix3x3::GetTranslation() const
{
	return SciVector2(data[6], data[7]);
}

SciVector3 SciMatrix3x3::GetRow(int row) const
{
	assert(row >= 0 && row < 3);

	switch(row)
	{
	case 0: return SciVector3(data[0], data[3], data[6]);
	case 1: return SciVector3(data[1], data[4], data[7]);
	case 2: return SciVector3(data[2], data[5], data[8]);
	default: return zero_vec3;
	}
}

SciVector3 SciMatrix3x3::GetColumn(int col) const
{
	assert(col >= 0 && col < 3);

	switch(col)
	{
	case 0: return SciVector3(data[0], data[1], data[2]);
	case 1: return SciVector3(data[3], data[4], data[5]);
	case 2: return SciVector3(data[6], data[7], data[8]);
	default: return zero_vec3;
	}
}
// End of Accsess functions

// Arithmetic operators
SciMatrix3x3 SciMatrix3x3::operator+(const SciMatrix3x3& other) const
{
	SciMatrix3x3 tmp;

	for(int i = 0; i < 9; ++i)
	{
		tmp.data[i] = data[i] + other.data[i];
	}

	return tmp;
}

SciMatrix3x3& SciMatrix3x3::operator+=(const SciMatrix3x3& other)
{
	for(int i = 0; i < 9; ++i)
	{
		data[i] += other.data[i];
	}

	return *this;
}

SciMatrix3x3 SciMatrix3x3::operator-(const SciMatrix3x3& other) const
{
	SciMatrix3x3 tmp;

	for(int i = 0; i < 9; ++i)
	{
		tmp.data[i] = data[i] - other.data[i];
	}

	return tmp;
}

SciMatrix3x3& SciMatrix3x3::operator-=(const SciMatrix3x3& other)
{
	for(int i = 0; i < 9; ++i)
	{
		data[i] -= other.data[i];
	}

	return *this;
}

SciMatrix3x3 SciMatrix3x3::operator*(const SciMatrix3x3& other) const
{
	SciMatrix3x3 tmp;

	tmp.data[0] = data[0]*other.data[0] + data[3]*other.data[1] + data[6]*other.data[2];
	tmp.data[3] = data[0]*other.data[3] + data[3]*other.data[4] + data[6]*other.data[5];
	tmp.data[6] = data[0]*other.data[6] + data[3]*other.data[7] + data[6]*other.data[8];

	tmp.data[1] = data[1]*other.data[0] + data[4]*other.data[1] + data[7]*other.data[2];
	tmp.data[4] = data[1]*other.data[3] + data[4]*other.data[4] + data[7]*other.data[5];
	tmp.data[7] = data[1]*other.data[6] + data[4]*other.data[7] + data[7]*other.data[8];

	tmp.data[2] = data[2]*other.data[0] + data[5]*other.data[1] + data[8]*other.data[2];
	tmp.data[5] = data[2]*other.data[3] + data[5]*other.data[4] + data[8]*other.data[5];
	tmp.data[8] = data[2]*other.data[6] + data[5]*other.data[7] + data[8]*other.data[8];

	return tmp;
}

SciMatrix3x3& SciMatrix3x3::operator*=(const SciMatrix3x3& other)
{
	SciMatrix3x3 tmp = *this * other;
	*this = tmp; // Does this copy an array? It should.
	return *this;
}

SciMatrix3x3 SciMatrix3x3::operator*(const float& scalar) const
{
	SciMatrix3x3 tmp;

	for(int i = 0; i < 9; ++i)
	{
		tmp.data[i] = data[i] * scalar;
	}

	return tmp;
}

SciMatrix3x3& SciMatrix3x3::operator*=(const float& scalar)
{
	for(int i = 0; i < 9; ++i)
	{
		data[i] *= scalar;
	}

	return *this;
}

// Post multiplication by column 3D vector
SciVector3 SciMatrix3x3::operator*(const SciVector3& vector) const
{
	SciVector3 tmp;
	tmp.x = data[0]*vector.x + data[3]*vector.y + data[6]*vector.z;
	tmp.y = data[1]*vector.x + data[4]*vector.y + data[7]*vector.z;
	tmp.z = data[2]*vector.x + data[5]*vector.y + data[8]*vector.z;
	return tmp;
}

SciMatrix3x3 SciMatrix3x3::operator/(const float& scalar) const
{
	float recip = 1.0f / scalar;
	return *this * recip;
}

SciMatrix3x3& SciMatrix3x3::operator/=(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this *= recip;
}
// End of Arithmetic Operators

// Relationsl operators
bool SciMatrix3x3::operator==(const SciMatrix3x3& other) const
{
	return !(bool)memcmp(this, &other, msize);
}

bool SciMatrix3x3::operator!=(const SciMatrix3x3& other) const
{
	return !(*this == other);
}
// End of Relationsl operators

// Core operations
SciMatrix3x3& SciMatrix3x3::Transpose()
{
	swap(data[3], data[1]);
	swap(data[6], data[2]);
	swap(data[7], data[5]);
	return *this;
}

// Inverse affine transformation matrix
SciMatrix3x3& SciMatrix3x3::AffineInverse()
{
	SciVector2 i(data[0], data[1]);
	SciVector2 j(data[3], data[4]);
	SciMatrix2x2 rot(i, j); // Rotation
	SciVector2 trans(data[6], data[7]); // Translation

	// Rotate negated vector by inversed rotation matrix
	trans = -trans * rot.Transpose();
	SetRotation(rot);
	SetTranslation(trans);

	return *this;
}

float SciMatrix3x3::Determinant() const
{
	float x = data[0] * (data[4]*data[8] - data[7]*data[5]);
	float y = data[3] * (data[1]*data[8] - data[7]*data[2]);
	float z = data[6] * (data[1]*data[5] - data[4]*data[2]);
	return x - y + z;
}

// Reset matrix to Identity matrix
SciMatrix3x3& SciMatrix3x3::Identity()
{
	data[0] = 1.0f;
	data[4] = 1.0f;
	data[8] = 1.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;
	data[3] = 0.0f;
	data[5] = 0.0f;
	data[6] = 0.0f;
	data[7] = 0.0f;

	return *this;
}

bool SciMatrix3x3::IsIdentity()
{
	return !(bool)memcmp(this, &identity, msize);
}
// End of Core operations

// Operator functions
SciMatrix3x3 operator*(const float& scalar, const SciMatrix3x3& matrix)
{
	return matrix * scalar;
}

// Its still post multiplication by column 3D vector
SciVector3 operator*(const SciVector3& vector, const SciMatrix3x3& matrix)
{
	return matrix * vector;
}
// End of Operator functions