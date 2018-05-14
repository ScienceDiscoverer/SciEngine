#include "Environment.h"

// Platform
// Constructors
Platform::Platform()
{
	sprite.Init(350, 20);
	sprite.Fill(SciPixel(0.88f, 0.88f, 0.88f));
	CalcMomInertInv(sprite);
	b_rec.CreateFromSprite(sprite);
	b_rec.SetMin(b_rec.Min() + SciVector2(-1.0f, -1.0f));
	b_rec.SetMax(b_rec.Max() + SciVector2(1.0f, 1.0f));
	pos = SciVector2(350.0f/2+0.5f, 10.0f);
	b_rec.Update(pos);
	is_moving = false;
	movable = false;
	mass = 5.972e+24f;
	elast = 0.63f;
	//elast = 0.0f;
}
// End of Constructors

// Interface functions
void Platform::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(pos);
	SciGame::game->renderer->Draw(sprite, alpha);
	SciGame::game->renderer->ResetWorld();
}
// End of Interface functions
// End of Platform