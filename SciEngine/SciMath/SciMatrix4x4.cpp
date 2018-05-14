#include <cassert>
#include <cstring>
#include "SciMatrix4x4.h"

// Constants
const size_t msize = sizeof(SciMatrix4x4);
const SciMatrix4x4 identity;

// Constructors
// Load Identity matrix
SciMatrix4x4::SciMatrix4x4()
{
	Identity();
}

// Construct from vectors (column-space) for post-mult. on column vector
SciMatrix4x4::SciMatrix4x4(const SciVector4& vec1, const SciVector4& vec2, const SciVector4& vec3, const SciVector4& vec4)
{
	SetColumns(vec1, vec2, vec3, vec4);
}

// Construct from 3x3 matrix
SciMatrix4x4::SciMatrix4x4(const SciMatrix3x3& matrix)
{
	SetZero();
	SetRotation(matrix);
	data[15] = 1.0f;
}

// Construct affine transformation matrix from 3x3 rotation, and 3D translation vector
SciMatrix4x4::SciMatrix4x4(const SciMatrix3x3& rot, const SciVector3& trans)
{
	SetRotation(rot);
	SetTranslation(trans);
	data[3] = 0.0f;
	data[7] = 0.0f;
	data[11] = 0.0f;
}
// End of Constructors

// Rule of 5
SciMatrix4x4::SciMatrix4x4(const SciMatrix4x4& other)
{
	memcpy(this, &other, msize);
}

SciMatrix4x4::SciMatrix4x4(SciMatrix4x4&& other)
{
	memcpy(this, &other, msize);
}

SciMatrix4x4& SciMatrix4x4::operator=(const SciMatrix4x4& other)
{
	memcpy(this, &other, msize);
	return *this;
}

SciMatrix4x4& SciMatrix4x4::operator=(SciMatrix4x4&& other)
{
	memcpy(this, &other, msize);
	return *this;
}
// End of Rule of 5

// Accsess functions
float& SciMatrix4x4::operator()(int row, int column)
{
	assert((row >= 0 && row < 4) && (column >= 0 && column < 4));
	return data[4*column + row]; // Column-major storage
}

const float& SciMatrix4x4::operator()(int row, int column) const
{
	assert((row >= 0 && row < 4) && (column >= 0 && column < 4));
	return data[4*column + row];
}

SciMatrix4x4& SciMatrix4x4::SetRows(const SciVector4& vec1, const SciVector4& vec2, const SciVector4& vec3, const SciVector4& vec4)
{
	SciVector4 vecs[4] = { vec1, vec2, vec3, vec4 };

	for(int i = 0; i < 4; ++i)
	{
		data[i] = vecs[i].x;
		data[i+4] = vecs[i].y;
		data[i+8] = vecs[i].z;
		data[i+12] = vecs[i].w;
	}

	return *this;
}

SciMatrix4x4& SciMatrix4x4::SetColumns(const SciVector4& vec1, const SciVector4& vec2, const SciVector4& vec3, const SciVector4& vec4)
{
	SciVector4 vecs[4] = { vec1, vec2, vec3, vec4 };

	for(int i = 0; i < 4; ++i)
	{
		data[i*4] = vecs[i].x;
		data[i*4+1] = vecs[i].y;
		data[i*4+2] = vecs[i].z;
		data[i*4+3] = vecs[i].w;
	}

	return *this;
}

// Set top left rotation block-matrix to matrix
SciMatrix4x4& SciMatrix4x4::SetRotation(const SciMatrix3x3& matrix)
{
	data[0] = matrix(0, 0);
	data[1] = matrix(1, 0);
	data[2] = matrix(2, 0);

	data[4] = matrix(0, 1);
	data[5] = matrix(1, 1);
	data[6] = matrix(2, 1);

	data[8] = matrix(0, 2);
	data[9] = matrix(1, 2);
	data[10] = matrix(2, 2);

	return *this;
}

// Set translation vector in affine matrix
SciMatrix4x4& SciMatrix4x4::SetTranslation(const SciVector3& vector)
{
	data[12] = vector.x;
	data[13] = vector.y;
	data[14] = vector.z;
	data[15] = 1.0f;

	return *this;
}

// Get top left rotation block-matrix from affine matrix
SciMatrix3x3 SciMatrix4x4::GetRotation() const
{
	SciVector3 x(data[0], data[1], data[2]);
	SciVector3 y(data[4], data[5], data[6]);
	SciVector3 z(data[8], data[9], data[10]);
	return SciMatrix3x3(x, y, z);
}

// Get translation vector from affine matrix
SciVector3 SciMatrix4x4::GetTranslation() const
{
	return SciVector3(data[12], data[13], data[14]);
}

SciVector4 SciMatrix4x4::GetRow(int row) const
{
	assert(row >= 0 && row < 4);

	switch(row)
	{
	case 0: return SciVector4(data[0], data[4], data[8], data[12]);
	case 1: return SciVector4(data[1], data[5], data[9], data[13]);
	case 2: return SciVector4(data[2], data[6], data[10], data[14]);
	case 3: return SciVector4(data[3], data[7], data[11], data[15]);
	default: return zero_vec4;
	}
}

SciVector4 SciMatrix4x4::GetColumn(int col) const
{
	assert(col >= 0 && col < 4);

	switch(col)
	{
	case 0: return SciVector4(data[0], data[1], data[2], data[3]);
	case 1: return SciVector4(data[4], data[5], data[6], data[7]);
	case 2: return SciVector4(data[8], data[9], data[10], data[11]);
	case 3: return SciVector4(data[12], data[13], data[14], data[15]);
	default: return zero_vec4;
	}
}
// End of Accsess functions

// Arithmetic operators
SciMatrix4x4 SciMatrix4x4::operator+(const SciMatrix4x4& other) const
{
	SciMatrix4x4 tmp;

	for(int i = 0; i < 16; ++i)
	{
		tmp.data[i] = data[i] + other.data[i];
	}

	return tmp;
}

SciMatrix4x4& SciMatrix4x4::operator+=(const SciMatrix4x4& other)
{
	for(int i = 0; i < 16; ++i)
	{
		data[i] += other.data[i];
	}

	return *this;
}

SciMatrix4x4 SciMatrix4x4::operator-(const SciMatrix4x4& other) const
{
	SciMatrix4x4 tmp;

	for(int i = 0; i < 16; ++i)
	{
		tmp.data[i] = data[i] - other.data[i];
	}

	return tmp;
}

SciMatrix4x4& SciMatrix4x4::operator-=(const SciMatrix4x4& other)
{
	for(int i = 0; i < 16; ++i)
	{
		data[i] -= other.data[i];
	}

	return *this;
}

SciMatrix4x4 SciMatrix4x4::operator*(const SciMatrix4x4& other) const
{
	SciMatrix4x4 tmp;

	for(int i = 0; i < 4; ++i)
	{
		const int c2 = i + 4; // Columns
		const int c3 = i + 8;
		const int c4 = i + 12;

		tmp.data[i]  = data[i]*other.data[0]  + data[c2]*other.data[1]  + data[c3]*other.data[2]  + data[c4]*other.data[3];
		tmp.data[c2] = data[i]*other.data[4]  + data[c2]*other.data[5]  + data[c3]*other.data[6]  + data[c4]*other.data[7];
		tmp.data[c3] = data[i]*other.data[8]  + data[c2]*other.data[9]  + data[c3]*other.data[10] + data[c4]*other.data[11];
		tmp.data[c4] = data[i]*other.data[12] + data[c2]*other.data[13] + data[c3]*other.data[14] + data[c4]*other.data[15];
	}

	return tmp;
}

SciMatrix4x4& SciMatrix4x4::operator*=(const SciMatrix4x4& other)
{
	SciMatrix4x4 tmp = *this * other;
	*this = tmp; // Does this copy an array? It should.
	return *this;
}

SciMatrix4x4 SciMatrix4x4::operator*(const float& scalar) const
{
	SciMatrix4x4 tmp;

	for(int i = 0; i < 16; ++i)
	{
		tmp.data[i] = data[i] * scalar;
	}

	return tmp;
}

SciMatrix4x4& SciMatrix4x4::operator*=(const float& scalar)
{
	for(int i = 0; i < 16; ++i)
	{
		data[i] *= scalar;
	}

	return *this;
}

// Post multiplication by column 3D vector
SciVector4 SciMatrix4x4::operator*(const SciVector4& vector) const
{
	SciVector4 tmp;

	tmp.x = data[0]*vector.x + data[4]*vector.y +  data[8]*vector.z + data[12]*vector.w;
	tmp.y = data[1]*vector.x + data[5]*vector.y +  data[9]*vector.z + data[13]*vector.w;
	tmp.z = data[2]*vector.x + data[6]*vector.y + data[10]*vector.z + data[14]*vector.w;
	tmp.w = data[3]*vector.x + data[7]*vector.y + data[11]*vector.z + data[15]*vector.w;

	return tmp;
}

SciMatrix4x4 SciMatrix4x4::operator/(const float& scalar) const
{
	float recip = 1.0f / scalar;
	return *this * recip;
}

SciMatrix4x4& SciMatrix4x4::operator/=(const float& scalar)
{
	float recip = 1.0f / scalar;
	return *this *= recip;
}
// End of Arithmetic Operators

// Relationsl operators
bool SciMatrix4x4::operator==(const SciMatrix4x4& other) const
{
	return !(bool)memcmp(this, &other, msize);
}

bool SciMatrix4x4::operator!=(const SciMatrix4x4& other) const
{
	return !(*this == other);
}
// End of Relationsl operators

// Core operations
// Inverse affine transformation matrix
SciMatrix4x4& SciMatrix4x4::AffineInverse()
{
	SciVector3 i(data[0], data[1], data[2]);
	SciVector3 j(data[4], data[5], data[6]);
	SciVector3 k(data[8], data[9], data[10]);
	SciMatrix3x3 rot(i, j, k); // Rotation
	SciVector3 trans(data[12], data[13], data[14]); // Translation

	// Rotate negated vector by inversed rotation matrix
	trans = -trans * rot.Transpose();
	SetRotation(rot);
	SetTranslation(trans);

	return *this;
}

// Reset matrix to Identity matrix
SciMatrix4x4& SciMatrix4x4::Identity()
{
	SetZero();
	data[0] = 1.0f;
	data[5] = 1.0f;
	data[10] = 1.0f;
	data[15] = 1.0f;

	return *this;
}

bool SciMatrix4x4::IsIdentity()
{
	return !(bool)memcmp(this, &identity, msize);
}
// End of Core operations

// Private functions
// Set matrix to zero matrix
void SciMatrix4x4::SetZero()
{
	for(int i = 0; i < 16; ++i)
	{
		data[i] = 0.0f;
	}
}
// End of Private functions

// Operator functions
SciMatrix4x4 operator*(const float& scalar, const SciMatrix4x4& matrix)
{
	return matrix * scalar;
}

// Its still post multiplication by column 3D vector
SciVector4 operator*(const SciVector4& vector, const SciMatrix4x4& matrix)
{
	return matrix * vector;
}
// End of Operator functions