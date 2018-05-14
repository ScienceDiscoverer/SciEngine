#ifndef SCICOLLISION2D_H
#define SCICOLLISION2D_H

#include "SciCollisionModule.h"
// IMPLEMENT COLLISIONS WITH LINES & LINE SEGMENTS (GROUND/WALLS)

class SciCollision2D : public SciCollisionModule
{
public:
	// Interface functions
	virtual void AddObject(ColEntPtr obj);
	virtual void EliminateObject(ColEntPtr obj);
	virtual const SciArray<Collision>& DetectCollisions();
	virtual const SciArray<Collision>* GetCollisions() { return &collisions; }

private:
	// Private functions
	void UpdateSweep();
	void DetectPixelCollisions(ColEntPtr a, ColEntPtr b);
	void AddCollision(ColEntPtr a, ColEntPtr b);
	const SciSprite& GetSprite(ColEntPtr obj);
	void ConstructSBRs(SciArray<SciAABR>& arr, const CollisionAABR& rc, ColEntPtr obj);

	// Data
	SciArray<Collision> collisions; // Detected collisions
	// Sweep-and-Prune system
	SciArray<ColEntPtr> pos_colliders; // Possible colliders
	SciArray<Endpoint> extents; // Object's extents on X-axis
};

#endif /* SCICOLLISION2D_H */