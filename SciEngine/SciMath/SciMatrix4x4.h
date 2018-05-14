#ifndef SCIMATRIX4X4_H
#define SCIMATRIX4X4_H

#include "SciVector4.h"
#include "SciMatrix3x3.h"

// Matrix assumes operations with column vectors
class SciMatrix4x4
{
public:
	// Constructors
	SciMatrix4x4(); // Loads Identity matrix
	SciMatrix4x4(const SciVector4& vec1, const SciVector4& vec2, const SciVector4& vec3, const SciVector4& vec4);
	SciMatrix4x4(const SciMatrix3x3& matrix); // Construct from 3x3 matrix
	SciMatrix4x4(const SciMatrix3x3& rot, const SciVector3& trans); // Construct affine matrix

	// Rule of 5
	~SciMatrix4x4() = default;
	SciMatrix4x4(const SciMatrix4x4& other);
	SciMatrix4x4(SciMatrix4x4&& other);
	SciMatrix4x4& operator=(const SciMatrix4x4& other);
	SciMatrix4x4& operator=(SciMatrix4x4&& other);

	// Accsess functions
	float& operator()(int row, int column);
	const float& operator()(int row, int column) const;
	SciMatrix4x4& SetRows(const SciVector4& vec1, const SciVector4& vec2, const SciVector4& vec3, const SciVector4& vec4);
	SciMatrix4x4& SetColumns(const SciVector4& vec1, const SciVector4& vec2, const SciVector4& vec3, const SciVector4& vec4);
	SciMatrix4x4& SetRotation(const SciMatrix3x3& matrix); // Set top left block-matrix to matrix
	SciMatrix4x4& SetTranslation(const SciVector3& vector); // Set translation vector in affine matrix
	SciMatrix3x3 GetRotation() const; // Get top left rotation block-matrix from affine matrix
	SciVector3 GetTranslation() const; // Get translation vector in affine matrix
	SciVector4 GetRow(int row) const;
	SciVector4 GetColumn(int col) const;
	const float* GetData() const { return data; }

	// Arithmetic operators
	SciMatrix4x4 operator+(const SciMatrix4x4& other) const;
	SciMatrix4x4& operator+=(const SciMatrix4x4& other);
	SciMatrix4x4 operator-(const SciMatrix4x4& other) const;
	SciMatrix4x4& operator-=(const SciMatrix4x4& other);
	SciMatrix4x4 operator*(const SciMatrix4x4& other) const;
	SciMatrix4x4& operator*=(const SciMatrix4x4& other);
	SciMatrix4x4 operator*(const float& scalar) const;
	SciMatrix4x4& operator*=(const float& scalar);
	SciVector4 operator*(const SciVector4& vector) const;
	SciMatrix4x4 operator/(const float& scalar) const;
	SciMatrix4x4& operator/=(const float& scalar);

	// Relational operators
	bool operator==(const SciMatrix4x4& other) const;
	bool operator!=(const SciMatrix4x4& other) const;

	// Core operations
	SciMatrix4x4& AffineInverse(); // Inverse affine transformation matrix
	SciMatrix4x4& Identity(); // Reset matrix to Identity matrix
	bool IsIdentity();

private:
	// Private functions
	void SetZero();
	
	// Data
	// Stored in column-major order
	// Accsessed as row-major - pretransposed for OpenGL/DX comparability
	float data[16];
};

// Operator functions
SciMatrix4x4 operator*(const float& scalar, const SciMatrix4x4& matrix);
SciVector4 operator*(const SciVector4& vector, const SciMatrix4x4& matrix);

#endif /* SCIMATRIX4X4_H */