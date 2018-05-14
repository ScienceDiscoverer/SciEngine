#ifndef SCIPIXEL_H
#define SCIPIXEL_H

struct SciPixel
{
	// Constructors
	SciPixel() = default;
	SciPixel(float red, float green, float blue, float alpha = 1.0f) : r(red), g(green), b(blue), a(alpha) {}

	// Operators
	SciPixel operator+(const SciPixel& other) const;
	SciPixel& operator+=(const SciPixel& other);
	SciPixel operator-(const SciPixel& other) const;
	SciPixel& operator-=(const SciPixel& other);
	// Memberwise multiplication
	SciPixel operator*(const SciPixel& other) const;
	SciPixel& operator*=(const SciPixel& other);
	SciPixel operator*(const float& scalar) const;
	SciPixel& operator*=(const float& scalar);
	SciPixel operator/(const float& scalar);
	SciPixel& operator/=(const float& scalar);

	// Core operations
	// Possible operations: sciClamp, interpolate etc.
	SciPixel ScaleColor(float scalar) const; // Scale only color, but not alpha
	SciPixel& InvertAlpha();
	float GetInverseAlpha() const; // Returns correct, clamped alpha

	// Data
	float r, g, b, a;
};

// Operator functions
SciPixel operator*(const float& scalar, const SciPixel& pixel);

// Color constants
const SciPixel red_color(1.0f, 0.0f, 0.0f, 1.0f);
const SciPixel green_color(0.0f, 1.0f, 0.0f, 1.0f);
const SciPixel blue_color(0.0f, 0.0f, 1.0f, 1.0f);
const SciPixel yellow_color(1.0f, 1.0f, 0.0f, 1.0f);
const SciPixel magenta_color(1.0f, 0.0f, 1.0f, 1.0f);
const SciPixel cyan_color(0.0f, 1.0f, 1.0f, 1.0f);
const SciPixel zero_color(0.0f, 0.0f, 0.0f, 0.0f);

#endif /* SCIPIXEL_H */