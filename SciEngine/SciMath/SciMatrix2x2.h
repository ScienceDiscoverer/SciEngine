#ifndef SCIMATRIX2X2_H
#define SCIMATRIX2X2_H

#include "SciVector2.h"

// Matrix assumes operations with column vectors
class SciMatrix2x2
{
public:
	// Constructors
	SciMatrix2x2(); // Loads Identity matrix
	SciMatrix2x2(const SciVector2& vec1, const SciVector2& vec2);
	SciMatrix2x2(float angle) { CalcRotation(angle); } // Create rotation matrix

	// Rule of 5
	~SciMatrix2x2() = default;
	SciMatrix2x2(const SciMatrix2x2& other);
	SciMatrix2x2(SciMatrix2x2&& other);
	SciMatrix2x2& operator=(const SciMatrix2x2& other);
	SciMatrix2x2& operator=(SciMatrix2x2&& other);

	// Accsess functions
	float& operator()(int row, int column);
	const float& operator()(int row, int column) const;
	SciMatrix2x2& SetRows(const SciVector2& vec1, const SciVector2& vec2);
	SciMatrix2x2& SetColumns(const SciVector2& vec1, const SciVector2& vec2);
	SciVector2 GetRow(int row) const;
	SciVector2 GetColumn(int col) const;
	const float* GetData() const { return data; }

	// Arithmetic operators
	SciMatrix2x2 operator+(const SciMatrix2x2& other) const;
	SciMatrix2x2& operator+=(const SciMatrix2x2& other);
	SciMatrix2x2 operator-(const SciMatrix2x2& other) const;
	SciMatrix2x2& operator-=(const SciMatrix2x2& other);
	SciMatrix2x2 operator*(const SciMatrix2x2& other) const;
	SciMatrix2x2& operator*=(const SciMatrix2x2& other);
	SciMatrix2x2 operator*(const float& scalar) const;
	SciMatrix2x2& operator*=(const float& scalar);
	SciVector2 operator*(const SciVector2& vector) const;
	SciMatrix2x2 operator/(const float& scalar) const;
	SciMatrix2x2& operator/=(const float& scalar);

	// Relational operators
	bool operator==(const SciMatrix2x2& other) const;
	bool operator!=(const SciMatrix2x2& other) const;

	// Core operations
	SciMatrix2x2& Transpose();
	float Determinant() const;
	SciMatrix2x2& CalcRotation(float angle); // Create count.clock. rotation matrix
	SciMatrix2x2& MakeClock(); // Make rot.m. clockwise
	SciMatrix2x2& MakeCountClock(); // Make rot.m. counterclockwise
	SciMatrix2x2& Identity(); // Reset matrix to Identity matrix
	bool IsIdentity();

private:
	// Data
	// Stored in column-major order
	// Accsessed as row-major - pretransposed for OpenGL/DX comparability
	float data[4];
};

// Operator functions
SciMatrix2x2 operator*(const float& scalar, const SciMatrix2x2& matrix);
SciVector2 operator*(const SciVector2& vector, const SciMatrix2x2& matrix);

// Global Constants
const SciMatrix2x2 rot_1_deg(3.1415926f/180.0f); // 1 degree rotation matrix

#endif /* SCIMATRIX2X2_H */