#ifndef SCICOLLISIONMODULE_H
#define SCICOLLISIONMODULE_H

#include "SciEntities2D.h"
#include "Utilities/SciArray.h"

// Typedefs
typedef SciCollisionEntity* ColEntPtr;

// Helper structs
// Collision that happened (Contact)
struct Collision
{
	// Constructors
	Collision() = default;
	Collision(ColEntPtr o_a, ColEntPtr o_b, const CollisionInfo& inf)
		: a(o_a), b(o_b), info(inf) {}

	// Data
	ColEntPtr a, b; // Collided objects
	CollisionInfo info; // Collision details
};

// Endpoint for Sweep-and-Prune system
struct Endpoint
{
	// Constructors
	Endpoint() = default;
	Endpoint(float ext, bool l, ColEntPtr o) : extent(ext), low(l), obj(o) {}

	// Data
	float extent;
	bool low; // Start endpoint if ture, Finish if 0
	ColEntPtr obj;
};

class SciCollisionModule
{
public:
	// Virtual destructor for polymorthic class
	virtual ~SciCollisionModule() = default;

	// Object registration for 2D module
	virtual void AddObject(ColEntPtr obj) = 0;
	// Many overloads for different bounding objects goes here...

	// Object registration for 3D module
	// Lots of overload for 3D bounding objects and mesh...

	// Object removal
	virtual void EliminateObject(ColEntPtr obj) = 0;

	// Main function of this system
	virtual const SciArray<Collision>& DetectCollisions() = 0;
	// Access to collisions array
	virtual const SciArray<Collision>* GetCollisions() = 0;
};

// Relational operators for Endpoint
inline bool operator<(const Endpoint& lhs, const Endpoint& rhs) { return lhs.extent < rhs.extent; }
inline bool operator>(const Endpoint& lhs, const Endpoint& rhs) { return rhs < lhs; }
inline bool operator<=(const Endpoint& lhs, const Endpoint& rhs) { return !(lhs > rhs); }
inline bool operator>=(const Endpoint& lhs, const Endpoint& rhs) { return !(lhs < rhs); }
inline bool operator==(const Endpoint& lhs, const Endpoint& rhs) { return lhs.extent == rhs.extent; }
inline bool operator!=(const Endpoint& lhs, const Endpoint& rhs) { return !(lhs == rhs); }

#endif /* SCICOLLISIONMODULE_H */