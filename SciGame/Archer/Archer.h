#ifndef ARCHER_H
#define ARCHER_H

#include <SciEntities2D.h>
#include <SciHierarchy.h>
#include <SciControls.h>

// Component classes
class LeftHand : public SciEntity
{
	friend class Archer;

public:
	// Constructors
	LeftHand();

	// Interface functions
	virtual void Update(float dt);
	virtual void Render(float alpha);

private:
	// Data
	SciVector2 w_pos;
	SciMatrix2x2 w_orient;
	MouseTracker m_stalker;
};

class RightHand : public SciEntity
{
	friend class Archer;

public:
	// Constructors
	RightHand();

	// Interface functions
	virtual void Update(float dt = -1.0f);
	virtual void Render(float alpha);

private:
	// Data
	SciVector2 w_pos;
	SciMatrix2x2 w_orient;
};

class Arrow : public SciPhysicsEntity
{
	friend class Archer;
	friend class Bow;

public:
	// Constructors
	Arrow();

	// Interface functions
	virtual void Update(float dt = -1.0f);
	virtual void Render(float alpha);

private:
	// Private functions
	void Penetrate();

	// Data
	SciVector2 w_pos;
	SciMatrix2x2 w_orient;
	SciVector2 cent_disp; // Displacement of center
	bool is_penning; // Arrow is penetrating something
	bool in_bow; // Arrow is inside bow
};

class Bow : public SciEntity
{
	friend class Archer;

public:
	// Constructors
	Bow();

	// Interface functions
	virtual void Update(float dt);
	virtual void Render(float alpha);

	// Core operations
	void InsertArrow(Arrow* arw) { ya = arw; }
	void ReleaseArrow();

private:
	// Data
	SciVector2 w_pos;
	SciMatrix2x2 w_orient;
	Arrow* ya;
	float draw; // Current draw power
	float last_draw;
	float relese_draw; // Draw power upon arrow relese
	float draw_spd; // Draw speed pixels/second
	float release_spd; // Release speed pixels/second
	SciArray<Arrow>* w_yas;
};

// Main class
class Archer : public SciPhysicsEntity
{
public:
	// Constructors
	Archer();
	
	// Interface functions
	virtual void Update(float dt);
	virtual void Render(float alpha);

	// Access functions
	void LinkWorldArrows(SciArray<Arrow>* w_arrs) { yumi.w_yas = w_arrs; }

private:
	// Data
	LeftHand l_hand;
	RightHand r_hand;
	Bow yumi;
	Arrow ya;
	SciHierarchy hier;
};

#endif /* ARCHER_H */