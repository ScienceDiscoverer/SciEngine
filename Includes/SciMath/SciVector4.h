#ifndef SCIVECTOR4_H
#define SCIVECTOR4_H

#include "SciVector3.h"

struct SciVector4
{
	// Constructors
	SciVector4() = default;
	SciVector4(float xVal, float yVal, float zVal, float wVal) : x(xVal), y(yVal), z(zVal), w(wVal) {}
	SciVector4(const SciVector3& vec3, float wVal) : x(vec3.x), y(vec3.y), z(vec3.z), w(wVal) {}
	SciVector4(const SciVector2& vec2, float zVal, float wVal) : x(vec2.x), y(vec2.y), z(zVal), w(wVal) {}

	// Operators
	SciVector4 operator+(const SciVector4& other) const;
	SciVector4& operator+=(const SciVector4& other);
	SciVector4 operator-(const SciVector4& other) const;
	SciVector4& operator-=(const SciVector4& other);
	SciVector4 operator*(const float& scalar) const;
	SciVector4& operator*=(const float& scalar);
	SciVector4 operator/(const float& scalar) const;
	SciVector4& operator/=(const float& scalar);
	SciVector4 operator-() const;

	// Core operations
	float Dot(const SciVector4& other) const; // Dot Product
	float MagSqr() const; // Magnitude squared
	float Mag() const; // Magnitude
	SciVector4& Normalize();
	SciVector4 Normalized() const;
	void Clean(); // Eliminate microscopic values
	void Set(float xVal, float yVal, float zVal, float wVal) { x = xVal; y = yVal; z = zVal; w = wVal; }
	bool CloseTo(const SciVector4& other) const; // Vectors are almost equal

	// Data
	float x, y, z, w;
};

// Arithmetic operators functions
SciVector4 operator*(const float& scalar, const SciVector4& vector);

// Relational operators functions
inline bool operator<(const SciVector4& lhs, const SciVector4& rhs);
inline bool operator>(const SciVector4& lhs, const SciVector4& rhs) { return rhs < lhs; }
inline bool operator<=(const SciVector4& lhs, const SciVector4& rhs) { return !(lhs > rhs); }
inline bool operator>=(const SciVector4& lhs, const SciVector4& rhs) { return !(lhs < rhs); }
inline bool operator==(const SciVector4& lhs, const SciVector4& rhs);
inline bool operator!=(const SciVector4& lhs, const SciVector4& rhs) { return !(lhs == rhs); }

// Constants
const SciVector4 zero_vec4(0.0f, 0.0f, 0.0f, 0.0f);

#endif /* SCIVECTOR4_H */
