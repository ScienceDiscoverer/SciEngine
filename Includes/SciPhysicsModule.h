#ifndef SCIPHYSICSMODULE_H
#define SCIPHYSICSMODULE_H

#include "SciEntities2D.h"
#include "Utilities/SciArray.h"
#include "SciCollisionModule.h"

// Typedefs
typedef SciPhysicsEntity* PhyEntPtr;

// Global Forces Enum
enum GlobalForce { Custom, Gravity };

// Helper structs
// Physical Collision
struct PhyCollision
{
	// Constructors
	PhyCollision() = default;
	PhyCollision(PhyEntPtr o_a, PhyEntPtr o_b, const CollisionInfo& inf)
		: a(o_a), b(o_b), info(inf) {}

	// Data
	PhyEntPtr a, b; // Collided objects
	CollisionInfo info; // Collision details
};

class SciPhysicsModule
{
public:
	// Virtual destructor for polymorthic class
	virtual ~SciPhysicsModule() = default;

	// Object registration for 2D module
	virtual void AddObject(PhyEntPtr obj) = 0;

	// Object removal
	virtual void EliminateObject(PhyEntPtr obj) = 0;

	// Physical inputter manipulation
	// Global forces must be added/removed only once, not in a loop
	// Note: all global forces must be provided for bodies with 1 kg mass
	virtual void AddGlobalForce(GlobalForce gforce, const SciVector2& force = zero_vec2) = 0;
	virtual void RemoveGlobalForce(GlobalForce gforce, const SciVector2& force = zero_vec2) = 0;
	// Overloads for 3D module here (global forces)...

	// Connecting Physics Engine to array of general collisions, must be done after construction
	virtual void ConnectCollisions(const SciArray<Collision>* cols) = 0;
	virtual const SciArray<PhyCollision>& GetPhyCollisions() = 0;

	// Set scale of the world (how much pixels is 1 m)
	virtual float WorldScale() = 0;
	// Air density must be in real world kg/m^3 scale
	virtual float ConfigAir(bool enabled, float air_den) = 0;

	// Main function of this system
	virtual void Integrate(float dt) = 0;
};

#endif /* SCIPHYSICSMODULE_H */