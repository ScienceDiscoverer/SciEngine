#ifndef SCIGAME_H
#define SCIGAME_H

#include "SciRenderModule.h"
#include "SciInputModule.h"
#include "SciCollisionModule.h"
#include "SciPhysicsModule.h"

class SciGame
{
public:
	// Standart constructor
	SciGame() : renderer(nullptr), collisioner(nullptr), physicist(nullptr), inputter(nullptr) {}

	// Virtual destructor for polymorthic class
	virtual ~SciGame();

	// Static function to be implemented in derived class file
	static bool Create();

	// Shutdown function
	static void Destroy();
	
	// Interface pure virtual functions
	virtual bool Initialize(ScreenRes scr) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(float alpha) = 0;

	// Global Public Data (Game instance pointer)
	static SciGame* game;

	// Game specific public data (game modules)
	SciRenderModule* renderer;
	SciInputModule* inputter;
	SciCollisionModule* collisioner;
	SciPhysicsModule* physicist;

};

#endif /* SCIGAME_H */