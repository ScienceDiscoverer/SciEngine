#ifndef SCIVECTOR3_H
#define SCIVECTOR3_H

#include "SciVector2.h"

struct SciVector3
{
	// Constructors
	SciVector3() = default;
	SciVector3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) {}
	SciVector3(const SciVector2& vec2, float zVal) : x(vec2.x), y(vec2.y), z(zVal) {}

	// Operators
	SciVector3 operator+(const SciVector3& other) const;
	SciVector3& operator+=(const SciVector3& other);
	SciVector3 operator-(const SciVector3& other) const;
	SciVector3& operator-=(const SciVector3& other);
	SciVector3 operator*(const float& scalar) const;
	SciVector3& operator*=(const float& scalar);
	SciVector3 operator/(const float& scalar) const;
	SciVector3& operator/=(const float& scalar);
	SciVector3 operator-() const;

	// Core operations
	float Dot(const SciVector3& other) const; // Dot Product
	SciVector3 Cross(const SciVector3& other) const; // Cross Product
	float MagSqr() const; // Magnitude squared
	float Mag() const; // Magnitude
	SciVector3& Normalize();
	SciVector3 Normalized() const;
	void Clean(); // Eliminate microscopic values
	void Set(float xVal, float yVal, float zVal) { x = xVal; y = yVal; z = zVal; }
	bool CloseTo(const SciVector3& other) const; // Vectors are almost equal

	// Data
	float x, y, z;
};

// Arithmetic operators functions
SciVector3 operator*(const float& scalar, const SciVector3& vector);

// Relational operators functions
inline bool operator<(const SciVector3& lhs, const SciVector3& rhs);
inline bool operator>(const SciVector3& lhs, const SciVector3& rhs) { return rhs < lhs; }
inline bool operator<=(const SciVector3& lhs, const SciVector3& rhs) { return !(lhs > rhs); }
inline bool operator>=(const SciVector3& lhs, const SciVector3& rhs) { return !(lhs < rhs); }
inline bool operator==(const SciVector3& lhs, const SciVector3& rhs);
inline bool operator!=(const SciVector3& lhs, const SciVector3& rhs) { return !(lhs == rhs); }

// Useful functions
float Distance(const SciVector3& point1, const SciVector3& point2);
float DistanceSqr(const SciVector3& point1, const SciVector3& point2);

// Constants
const SciVector3 zero_vec3(0.0f, 0.0f, 0.0f);

#endif /* SCIVECTOR3_H */
