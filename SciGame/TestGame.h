#ifndef TESTGAME_H
#define TESTGAME_H

#include <SciGame.h>
#include <SciRenderer2D.h>
#include <SciInputModule.h>
#include <SciCollision2D.h>
#include <SciEntities2D.h>
#include <SciHierarchy.h>
#include <SciControls.h>

class Onna : public SciCollisionEntity
{
public:
	// Constructors
	Onna() : cols(nullptr) {}
	Onna(const SciArray<Collision>* c, int skin);

	// Interface functions
	virtual void Update(float dt = -1.0f);
	virtual void Render(float alpha);

protected:
	// Data
	const SciArray<Collision>* cols;
	bool is_collid;
};

class OnnaNPC : public Onna
{
public:
	// Constructors
	OnnaNPC() = default;
	OnnaNPC(const SciArray<Collision>* c, int skin) : Onna(c, skin) { pos = SciVector2(50.0f, 100.0f); b_rec.Update(SciVector2(50.0f, 100.0f)); }

	// Interface functions
	virtual void Update(float dt = -1.0f);
	virtual void Render(float alpha);
};

class Turret : public SciEntity
{
	friend class Tank;
public:
	// Constructors
	Turret();

	// Interface functions
	virtual void Update(float dt);
	virtual void Render(float alpha);

private:
	// Data
	SciVector2 w_pos;
	SciMatrix2x2 w_orient;
	MouseTracker m_stalker;
};

class Tank : public SciEntity
{
public:
	// Constructors
	Tank();

	// Interface functions
	virtual void Update(float dt);
	virtual void Render(float alpha);

private:
	// Data
	SciHierarchy hier;
	Turret turr;
};

class TestGame : public SciGame
{
public:
	// Constructors
	TestGame();

	// Virtual interface functions
	virtual bool Initialize(ScreenRes scr);
	virtual void Update(float dt);
	virtual void Render(float alpha);

private:
	// Data
	Onna yuki;
	OnnaNPC horo;
	Tank tiger;
	SciVector2 fog_trans; // Dummy transform for fog
	SciResourceNan<SciPixel> fog; // Dummy fog
	bool fog_patrol;
};

#endif /* TESTGAME_H */
