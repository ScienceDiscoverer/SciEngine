#ifndef SCIVECTOR2_H
#define SCIVECTOR2_H

struct SciVector2
{
	// Constructors
	SciVector2() = default;
	SciVector2(float xVal, float yVal) : x(xVal), y(yVal) {}

	// Operators
	SciVector2 operator+(const SciVector2& other) const;
	SciVector2& operator+=(const SciVector2& other);
	SciVector2 operator-(const SciVector2& other) const;
	SciVector2& operator-=(const SciVector2& other);
	SciVector2 operator*(const float& scalar) const;
	SciVector2& operator*=(const float& scalar);
	SciVector2 operator/(const float& scalar) const;
	SciVector2& operator/=(const float& scalar);
	SciVector2 operator-() const;

	// Core operations
	float Dot(const SciVector2& other) const; // Dot Product
	float PerpDot(const SciVector2& other) const; // Perpendicular Dot Product
	float MagSqr() const; // Magnitude squared
	float Mag() const; // Magnitude
	SciVector2& Normalize();
	SciVector2 Normalized() const;
	SciVector2 Perp() const; // Get counterclockwise perpendicular to this vector
	void Clean(); // Eliminate microscopic values
	void Set(float xVal, float yVal) { x = xVal; y = yVal; }
	bool CloseTo(const SciVector2& other) const; // Vectors are almost equal

	// Data
	float x, y;
};

// Arithmetic operators functions
SciVector2 operator*(const float& scalar, const SciVector2& vector);

// Relational operators functions
inline bool operator<(const SciVector2& lhs, const SciVector2& rhs) { return lhs.x < rhs.x && lhs.y < rhs.y; }
inline bool operator>(const SciVector2& lhs, const SciVector2& rhs) { return rhs < lhs; }
inline bool operator<=(const SciVector2& lhs, const SciVector2& rhs) { return !(lhs > rhs); }
inline bool operator>=(const SciVector2& lhs, const SciVector2& rhs) { return !(lhs < rhs); }
inline bool operator==(const SciVector2& lhs, const SciVector2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
inline bool operator!=(const SciVector2& lhs, const SciVector2& rhs) { return !(lhs == rhs); }

// Useful functions
float Distance(const SciVector2& point1, const SciVector2& point2);
float DistanceSqr(const SciVector2& point1, const SciVector2& point2);

// Constants
const SciVector2 zero_vec2(0.0f, 0.0f);

#endif /* SCIVECTOR2_H */
