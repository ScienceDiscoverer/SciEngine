#ifndef ARCHERGAME_H
#define ARCHERGAME_H

#include <SciGame.h>
#include <SciInputModule.h>
#include <SciRenderer2D.h>
#include <SciCollision2D.h>
#include <SciPhysics2D.h>
#include "Archer.h"
#include "Environment.h"

class ArcherGame : public SciGame
{
public:
	// Constructors
	ArcherGame();

	// Virtual interface functions
	virtual bool Initialize(ScreenRes scr);
	virtual void Update(float dt);
	virtual void Render(float alpha);

private:
	// Data
	Archer yumiko;
	Platform earth;
	SciArray<Arrow> w_arrows; // Array of world arrows
};

#endif /* ARCHERGAME_H */