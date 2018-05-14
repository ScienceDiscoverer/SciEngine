#include "SciGame.h"

// Static data initialization
SciGame* SciGame::game = nullptr;
// End of Static data initialization

// Virtual destructor for polymorthic class
SciGame::~SciGame()
{
	if(renderer)
	{
		delete renderer;
	}

	if(inputter)
	{
		delete inputter;
	}

	if(collisioner)
	{
		delete collisioner;
	}

	if(physicist)
	{
		delete physicist;
	}
}
// End of Virtual destructor

// Static functions
// Shutdown function
void SciGame::Destroy()
{
	if(game)
	{
		delete game;
		game = nullptr;
	}
}
// End of Static functions