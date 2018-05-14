#ifndef SCIENTITIES2D_H
#define SCIENTITIES2D_H

#include "Utilities/SciResourceNan.h"
#include "Utilities/SciBoundingObjects2D.h"
#include "SciRenderModule.h"

// Friend Classes forward declarations
class SciCollision2D;
class SciPhysics2D;

// Base Game Entity without any collision or physical properties
class SciEntity
{
	// Friend Classes
	friend class SciCollision2D;
	friend class SciPhysics2D;

public:
	// Standart constructor
	SciEntity() : ex_sprite(nullptr), pos(zero_vec2) {}

	// Virtual destructor for polymorthic class
	virtual ~SciEntity() = default; // All derived hierarchy will inherit virt. dest.

	// Interface functions
	virtual void Update(float dt = -1.0f) = 0;
	virtual void Render(float alpha) = 0;

protected:
	// Core operations
	SciMatrix3x3 ModWorld() const;
	void SetRotSprite(const SciSprite& os, const SciVector2& ct, const SciMatrix2x2 or);
	void SetRotSprite(const SciSprite& os);
	void SetRotSprite();
	void RotateSprite(const SciSprite& orig, const SciMatrix2x2& or, const SciVector2& ct);
	void RotateSprite(const SciSprite& orig, const SciMatrix2x2& or);
	void RotateSprite(const SciSprite& orig);
	void RotateSprite();

	// Data
	SciSprite sprite;
	const SciSprite* ex_sprite; // External (shared) sprite
	SciSprite rot_sprite; // Rotated sprite
	SciVector2 pos; // Position
	SciMatrix2x2 orient; // Orientation
	SciMatrix2x2 past_orient; // Orientation in past frame
};

// Advanced Game Entity that can be used to detect collisions with oter coll. or phy. entities
class SciCollisionEntity : public SciEntity
{
	// Friend Classes
	friend class SciCollision2D;

public:
	// Standart constructor
	SciCollisionEntity() : is_moving(true) {}

	// Interface functions
	virtual void Update(float dt = -1.0f) = 0;
	virtual void Render(float alpha) = 0;

	// Collision update functions
	void UpdateBound(const SciVector2& trans);

protected:
	// Data
	SciAABR b_rec; // Aix-Aligned Bounding Rectangle
	bool is_moving;
};

// Compleat Game Entity used in physical simulations
class SciPhysicsEntity : public SciCollisionEntity
{
	// Friend Classes
	friend class SciPhysics2D;

public:
	// Standart constructor
	SciPhysicsEntity() : vel(zero_vec2), forces(zero_vec2), mass(1.0f),
		ang_vel(0.0f), torqs(0.0f), ang_mom(0.0f), mom_inert_inv(0.0f),
		c_pres(zero_vec2), real_w(1.0f), real_h(1.0f), drag_c(0.0f),
		lift_c(0.0f), elast(1.0f), movable(true), rot_disabled(false),
		last_frame_accel(zero_vec2) {}

	// Interface functions
	virtual void Update(float dt = -1.0f) = 0;
	virtual void Render(float alpha) = 0;

protected:
	// Core operations
	void CalcMomInertInv(const SciSprite& spr); // Define desired mass before calling

	// Data
	// Linear Dynamics
	SciVector2 vel; // Velocity
	SciVector2 forces; // Total local Forces
	float mass; // Must be >= 0
	// Rotational Dynamics
	float ang_vel; // Angular Velocity
	float torqs; // Total local Torques
	float ang_mom; // Angular Momentum
	float mom_inert_inv; // Moment of Inertia inversed
	// Fluid Dynamics
	SciVector2 c_pres; // Center of pressure
	float real_w; // Real Width
	float real_h; // Real Height
	float drag_c; // Drag coefficient
	float lift_c; // Lift coefficient
	// Ohter physical properties
	float elast; // Elasticity must be >= 0 && <= 1
	bool movable; // Object can/can't move
	bool rot_disabled; // Rotationa dynamics disabled for this object
	SciVector2 last_frame_accel; // Last frame acceleration
};

#endif /* SCIENTITIES2D_H */