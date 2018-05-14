#ifndef SCIMATRIX3X3_H
#define SCIMATRIX3X3_H

#include "SciVector3.h"
#include "SciMatrix2x2.h"

// Matrix assumes operations with column vectors
class SciMatrix3x3
{
public:
	// Constructors
	SciMatrix3x3(); // Loads Identity matrix
	SciMatrix3x3(const SciVector3& vec1, const SciVector3& vec2, const SciVector3& vec3);
	SciMatrix3x3(const SciMatrix2x2& matrix); // Construct from 2x2 matrix
	SciMatrix3x3(const SciMatrix2x2& rot, const SciVector2& trans); // Construct affine matrix

	// Rule of 5
	~SciMatrix3x3() = default;
	SciMatrix3x3(const SciMatrix3x3& other);
	SciMatrix3x3(SciMatrix3x3&& other);
	SciMatrix3x3& operator=(const SciMatrix3x3& other);
	SciMatrix3x3& operator=(SciMatrix3x3&& other);

	// Accsess functions
	float& operator()(int row, int column);
	const float& operator()(int row, int column) const;
	SciMatrix3x3& SetRows(const SciVector3& vec1, const SciVector3& vec2, const SciVector3& vec3);
	SciMatrix3x3& SetColumns(const SciVector3& vec1, const SciVector3& vec2, const SciVector3& vec3);
	SciMatrix3x3& SetRotation(const SciMatrix2x2& matrix); // Set top left rotation block-matrix to matrix
	SciMatrix3x3& SetTranslation(const SciVector2& vector); // Set translation vector in affine matrix
	SciMatrix2x2 GetRotation() const; // Get top left rotation block-matrix from affine matrix
	SciVector2 GetTranslation() const; // Get translation vector from affine matrix
	SciVector3 GetRow(int row) const;
	SciVector3 GetColumn(int col) const;
	const float* GetData() const { return data; }

	// Arithmetic operators
	SciMatrix3x3 operator+(const SciMatrix3x3& other) const;
	SciMatrix3x3& operator+=(const SciMatrix3x3& other);
	SciMatrix3x3 operator-(const SciMatrix3x3& other) const;
	SciMatrix3x3& operator-=(const SciMatrix3x3& other);
	SciMatrix3x3 operator*(const SciMatrix3x3& other) const;
	SciMatrix3x3& operator*=(const SciMatrix3x3& other);
	SciMatrix3x3 operator*(const float& scalar) const;
	SciMatrix3x3& operator*=(const float& scalar);
	SciVector3 operator*(const SciVector3& vector) const;
	SciMatrix3x3 operator/(const float& scalar) const;
	SciMatrix3x3& operator/=(const float& scalar);

	// Relational operators
	bool operator==(const SciMatrix3x3& other) const;
	bool operator!=(const SciMatrix3x3& other) const;

	// Core operations
	SciMatrix3x3& Transpose();
	SciMatrix3x3& AffineInverse(); // Inverse affine transformation matrix
	float Determinant() const;
	SciMatrix3x3& Identity(); // Reset matrix to Identity matrix
	bool IsIdentity();

private:
	// Data
	// Stored in column-major order
	// Accsessed as row-major - pretransposed for OpenGL/DX comparability
	float data[9];
};

// Operator functions
SciMatrix3x3 operator*(const float& scalar, const SciMatrix3x3& matrix);
SciVector3 operator*(const SciVector3& vector, const SciMatrix3x3& matrix);

#endif /* SCIMATRIX3X3_H */
