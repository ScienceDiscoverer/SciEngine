#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <SciGame.h>
#include <SciRenderer2D.h>

class Platform : public SciPhysicsEntity
{
public:
	// Constructors
	Platform();

	// Interface functions
	virtual void Update(float dt = -1.0f) {}
	virtual void Render(float alpha);
};

#endif /* ENVIRONMENT_H */