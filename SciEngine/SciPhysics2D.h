#ifndef SCIPHYSICS2D_H
#define SCIPHYSICS2D_H

#include "SciPhysicsModule.h"

class SciPhysics2D : public SciPhysicsModule
{
public:
	// Constructors
	SciPhysics2D() : g_forces(zero_vec2), gen_cols(nullptr), world_scale(20.0f),
		air_enabled(true), air_dens(1.1839f * 0.05f) {} // Earth air, 25degC, 1atm
	SciPhysics2D(float w_scale) : g_forces(zero_vec2), gen_cols(nullptr), world_scale(w_scale),
		air_enabled(true), air_dens(0.11f/world_scale) {} // Earth air, 25degC, 1atm

	// Interface functions
	virtual void AddObject(PhyEntPtr obj) { objs.Push(obj); }
	virtual void EliminateObject(PhyEntPtr obj) { objs.Remove(obj); }
	virtual void AddGlobalForce(GlobalForce gforce, const SciVector2& force = zero_vec2);
	virtual void RemoveGlobalForce(GlobalForce gforce, const SciVector2& force = zero_vec2);
	virtual void ConnectCollisions(const SciArray<Collision>* cols) { gen_cols = cols; }
	virtual const SciArray<PhyCollision>& GetPhyCollisions() { return phy_cols; }
	virtual float WorldScale() { return world_scale; }
	virtual float ConfigAir(bool enabled, float air_den);
	virtual void Integrate(float dt);

private:
	// Private functions
	void CollisionResponse();
	void FilterCollisions(); // Separate Physical Collisions from General Collisions

	// Data
	SciArray<PhyEntPtr> objs; // Objects included in physical simulation
	SciVector2 g_forces; // Global Forces
	float world_scale; // How many pixels is 1 m
	bool air_enabled; // Aerodynamic simulation enabled
	float air_dens; // Air density (kg/pix^2)
	// Collision Data
	const SciArray<Collision>* gen_cols; // Array of General Collisions
	SciArray<PhyCollision> phy_cols; // Array of Physical Collisions
};

#endif /* SCIPHYSICS2D_H */