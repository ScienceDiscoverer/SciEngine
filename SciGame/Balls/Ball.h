#ifndef BALL_H
#define BALL_H

#include <SciGame.h>
#include <SciEntities2D.h>
#include <SciCollisionModule.h>
#include <SciPhysicsModule.h>

class Ball : public SciPhysicsEntity
{
public:
	// Constructors
	Ball() : res(nullptr) {}
	Ball(const SciSprite* s, const ScreenRes* scr_res);

	// Interface functions
	virtual void Update(float dt = -1.0f);
	virtual void Render(float alpha);

	// Access functions
	void ApplyForce(const SciVector2& f) { forces = f; }
	void ApplyTorque(float tq) { torqs = tq; }

private:
	// Data
	const ScreenRes* res;
};

//class Platform : public SciPhysicsEntity
//{
//public:
//	// Constructors
//	Platform();
//	Platform(int i);
//	
//	void SetSprite(SciSprite spr) { sprite = spr; b_rec.CreateFromSprite(sprite); }
//
//	// Interface functions
//	virtual void Update(float dt = -1.0f) {}
//	virtual void Render(float alpha);
//};

#endif /* BALL_H */