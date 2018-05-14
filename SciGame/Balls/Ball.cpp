#include <cstdlib>
#include <SciMath.h>
#include "Ball.h"

//// Constatns
//const float da = pi/100.0f; // Delta angle
//SciMatrix2x2 ang_rot;

// Constructors
Ball::Ball(const SciSprite* s, const ScreenRes* scr_res) : res(scr_res)
{
	ex_sprite = s;
	CalcMomInertInv(*s);
	SetRotSprite(*s);
	b_rec.CreateFromSprite(rot_sprite);

	// Spawn ball in the center of window/screen
	float scr_c_x = (float)res->w / 2;
	float scr_c_y = (float)res->h / 2;
	pos = SciVector2(scr_c_x, scr_c_y);
	b_rec.Update(pos);

	//// Generate velocity
	float random = (float)rand() / RAND_MAX;
	vel.x = 10.0f + random * 16.0f;
	random = (float)rand() / RAND_MAX;
	vel.y = 10.0f + random * 16.0f;

	// Tweek physical properties
	elast = 0.73f;
	//elast = 0.0f;

	/*float cos_a, sin_a;
	sciCosSin(da, cos_a, sin_a);
	ang_rot(0, 0) = cos_a;
	ang_rot(0, 1) = sin_a;
	ang_rot(1, 0) = -sin_a;
	ang_rot(1, 1) = cos_a;*/
}
// End of Constructors

// Main functions
void Ball::Update(float dt)
{	
	// Keep balls (boxes) inside screen (simulate "walls")
	//if(b_rec.Min().x < 0.0f && vel.x < 0.0f)
	//{
	//	vel.x = -vel.x; // This effectively turns velocity by pi/2 radians from "wall"!
	//}
	//if(b_rec.Max().x > res->w && vel.x > 0.0f)
	//{
	//	vel.x = -vel.x;
	//}
	//if(b_rec.Min().y < 0.0f && vel.y < 0.0f)
	//{
	//	vel.y = -vel.y;
	//}
	//if(b_rec.Max().y > res->h && vel.y > 0.0f)
	//{
	//	vel.y = -vel.y;
	//}

	/*orient *= ang_rot;*/

	RotateSprite(*ex_sprite);
}

void Ball::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(pos);

	SciGame::game->renderer->Draw(rot_sprite, alpha);

	SciGame::game->renderer->ResetWorld();
}
// End of Main functions

// Platform
//Platform::Platform()
//{
//	sprite.Init(350, 20);
//	sprite.Fill(SciPixel(0.88f, 0.88f, 0.88f));
//	CalcMomInertInv(sprite);
//	b_rec.CreateFromSprite(sprite);
//	b_rec.SetMin(b_rec.Min() + SciVector2(-1.0f, -1.0f));
//	b_rec.SetMax(b_rec.Max() + SciVector2(1.0f, 1.0f));
//	pos = SciVector2(350.0f/2+0.5f, 10.0f);
//	b_rec.Update(pos);
//	is_moving = false;
//	movable = false;
//	mass = 5.972e+24f;
//	elast = 0.63f;
//	//elast = 0.0f;
//}
//
//Platform::Platform(int i)
//{
//	sprite.Init(20, 350);
//	b_rec.CreateFromSprite(sprite);
//	b_rec.SetMin(b_rec.Min() + SciVector2(-1.0f, -1.0f));
//	b_rec.SetMax(b_rec.Max() + SciVector2(1.0f, 1.0f));
//	switch(i)
//	{
//	case 0:
//		pos = SciVector2(10.0f, 350.0f/2+0.5f);
//		break;
//	case 1:
//		pos = SciVector2(340.5f, 350.0f/2+0.5f);
//		break;
//	default:
//		break;
//	}
//
//	b_rec.Update(pos);
//	sprite.Fill(SciPixel(0.88f, 0.88f, 0.88f));
//	CalcMomInertInv(sprite);
//	is_moving = false;
//	movable = false;
//	mass = 5.972e+24f;
//	elast = 0.63f;
//	//elast = 0.00f;
//}
//
//void Platform::Render(float alpha)
//{
//	SciGame::game->renderer->TranslateWorld(pos);
//	SciGame::game->renderer->Draw(sprite, alpha);
//	SciGame::game->renderer->ResetWorld();
//}
// End of Platfrom