#ifndef SCIBOUNDINGOBJECTS2D_H
#define SCIBOUNDINGOBJECTS2D_H

#include "../SciMath/SciVector2.h"
#include "SciResourceNan.h"

// Helper structs
// Collision information
struct CollisionInfo
{
	// Constructors
	CollisionInfo() = default;
	CollisionInfo(const SciVector2& norm, const SciVector2& p, float pent)
		: normal(norm), point(p), pen(pent) {}

	// Data
	SciVector2 normal; // Collision normal
	SciVector2 point; // Collision point
	float pen; // Penetration Depth
};

struct CollisionAABR
{
	CollisionInfo info;
	float pen_x, pen_y; // Vertical/horisontal penetration
	SciVector2 min; // Minimum point of box created by penetration
};

// Bounding Circle
class SciBCircle
{
public:
	// Constructors
	SciBCircle() : center_(zero_vec2), radius_(0.0f) {}
	SciBCircle(SciVector2 center, float radius) : center_(center), radius_(radius) {}

	// Access functions
	const SciVector2& Center() const { return center_; }
	float Radius() const { return radius_; }
	void SetCenter(const SciVector2& cent) { center_ = cent; }
	void SetRadius(float rad) { radius_ = rad; }
	void Update(const SciVector2& trans) { center_ += trans; }

	// Collision detection functions
	bool IsColliding(const SciBCircle& other) const;
	// ADD COLLISION WITH LINE, RAY, LINE SEGMENT LATER.......

private:
	// Data
	SciVector2 center_;
	float radius_;
};

// Aix-Aligned Bounding Rectangle
class SciAABR
{
public:
	// Constructors
	SciAABR() : minima(zero_vec2), maxima(zero_vec2){}
	SciAABR(const SciVector2& min, const SciVector2& max) : minima(min), maxima(max) {}
	SciAABR(const SciSprite& spr) { CreateFromSprite(spr); }
	
	// Access functions
	const SciVector2& Min() const { return minima; }
	const SciVector2& Max() const { return maxima; }
	void SetMin(const SciVector2& vec) { minima = vec; }
	void SetMax(const SciVector2& vec) { maxima = vec; }
	void Update(const SciVector2& trans);
	void CreateFromSprite(const SciSprite& spr, const SciVector2& pos = zero_vec2);

	// Collision detection functions
	bool IsColliding(const SciAABR& other) const;
	CollisionAABR GetFullCollision(const SciAABR& other) const;
	SciAABR GetCollisionRect(const SciAABR& other) const;
	// ADD COLLISION WITH LINE, RAY, LINE SEGMENT LATER.......

private:
	// Data
	SciVector2 minima;
	SciVector2 maxima;
};

#endif /* SCIBOUNDINGOBJECTS2D_H */