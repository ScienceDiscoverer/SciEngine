#include <SciMath.h>
#include "TestGame.h"
#include "ActiveGame.h"
////////////////// DEBUG INCLUDES ///////////////////
//#define SCI_DEBUG
#ifdef SCI_DEBUG
#include <windows.h>
#include <string>
#include <fstream>
#include "SciGame.h"
typedef std::string str;

HWND debug, d_info;
bool debug_on = false;
int frames = 0;
str out;


template <class T>
str tstr(T srs) { return std::to_string(srs); }
template <>
str tstr(SciVector2 srs) { return str("[")+std::to_string(srs.x)+str("|")+std::to_string(srs.y)+str("]"); }
template <>
str tstr(bool boo) { return boo ? str("TRUE") : str("FALSE"); }
void drawVecDebug(SciVector2 vec, SciVector2 pos, const SciPixel& col = blue_color)
{
	SciGame::game->renderer->TranslateWorld(pos);
	SciGame::game->renderer->Draw(drawLine(vec, col), 1.0f);
	SciGame::game->renderer->ResetWorld();
}

float tot_ang;
SciVector2 dirg, w_posg, mouseg;
#endif /* SCI_DEBUG */
/////////////////// DEBUG INCLUDES ///////////////////
/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

w_posg = w_pos;
mouseg = mouse;
dirg = w_orient.GetColumn(1);

SetWindowTextA(d_info, out.c_str());
out.clear();

/*if(SciGame::game->inputter->KeyIsDown('s'))
{
std::ofstream ofs("debug.txt");
ofs << out;
ofs.close();
}*/

#endif /* SCI_DEBUG */
/////////////////////////////////// DEBUGING //////////////////////////////////

// Globals
const float spd = 0.08f;
const float da = pi/300.0f; // Delta angle
SciMatrix2x2 ang_vel;

//////// Onna ////////
// Constructors
Onna::Onna(const SciArray<Collision>* c, int skin) : cols(c), is_collid(false)
{
	switch(skin)
	{
	case 0: sprite.LoadFile("img/onna.tga", 14, 16); break;
	case 1: sprite.LoadFile("img/rott.tga", 50, 50); break;
	case 2: sprite.LoadFile("img/arrow_prot.tga", 30, 8); break;
	default: break;
	}
	// Set up rotated sprite
	SetRotSprite(sprite);
	// Initialize bounding rectangle to correct size
	b_rec.CreateFromSprite(rot_sprite);
}

// Interface functions
void Onna::Update(float dt)
{
	is_collid = false;
	const SciArray<Collision>& col = *cols;
	for(int i = 0; i < cols->Size(); ++i)
	{
		if(col[i].a == this || col[i].b == this)
		{
			is_collid = true;
		}
	}

	SciVector2 vel(zero_vec2);

	if(SciGame::game->inputter->KeyIsDown('i'))
	{
		vel.y = spd;
	}

	if(SciGame::game->inputter->KeyIsDown('k'))
	{
		vel.y = -spd;
	}

	if(SciGame::game->inputter->KeyIsDown('l'))
	{
		vel.x = spd;
	}

	if(SciGame::game->inputter->KeyIsDown('j'))
	{
		vel.x = -spd;
	}

	if(vel.x || vel.y)
	{
		vel.Normalize();
		vel *= spd;
		pos += vel;
		b_rec.Update(vel);
	}

	if(SciGame::game->inputter->KeyIsDown('u'))
	{
		orient *= ang_vel;
	}
	if(SciGame::game->inputter->KeyIsDown('o'))
	{
		orient *= ang_vel.Transpose();
		ang_vel.Transpose();
	}

	// Update sprite rotation
	RotateSprite(sprite);
}

void Onna::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(pos);
	if(is_collid)
	{
		SciSprite col_spr = rot_sprite;
		applyFilter(col_spr, red_color);
		SciGame::game->renderer->Draw(col_spr, alpha);
	}
	else
	{
		SciGame::game->renderer->Draw(rot_sprite, alpha);
	}
	SciGame::game->renderer->ResetWorld();
}
//////// End of Onna ////////

//////// OnnaNPC ////////
// Interface functions
void OnnaNPC::Update(float dt)
{
	is_collid = false;
	const SciArray<Collision>& col = *cols;
	for(int i = 0; i < cols->Size(); ++i)
	{
		if(col[i].a == this || col[i].b == this)
		{
			is_collid = true;
		}
	}
}

void OnnaNPC::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(pos);
	if(is_collid)
	{
		SciSprite col_spr = rot_sprite;
		applyFilter(col_spr, red_color);
		SciGame::game->renderer->Draw(col_spr, alpha);
	}
	else
	{
		SciGame::game->renderer->Draw(rot_sprite, alpha);
	}
	SciGame::game->renderer->ResetWorld();
}
//////// End of OnnaNPC ////////

//////// Tank ////////
// Constructors
Turret::Turret() : w_pos(pos), w_orient(orient)
{
	sprite.LoadFile("img/tank_gun.tga", 18, 50);
	SetRotSprite();
	m_stalker.Link(&orient, &w_orient, &w_pos);
}

// Interface functions
void Turret::Update(float dt)
{
	m_stalker.Follow(dt);
}

void Turret::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(w_pos);
	SciGame::game->renderer->Draw(rot_sprite, alpha);
	SciGame::game->renderer->ResetWorld();
}

Tank::Tank()
{
	sprite.LoadFile("img/tank_body.tga", 30, 30);
	SetRotSprite();

	Transforms base;
	base.l_orient = &orient;
	base.l_pos = &pos;
	base.w_orient = &orient;
	base.w_pos = &pos;
	hier.AddNode(-1, base);

	Transforms tur;
	tur.l_orient = &turr.orient;
	tur.l_pos = &turr.pos;
	tur.w_orient = &turr.w_orient;
	tur.w_pos = &turr.w_pos;
	hier.AddNode(0, tur);
}

// Interface functions
void Tank::Update(float dt)
{
	turr.m_stalker.BaseStill();
	if(SciGame::game->inputter->KeyIsDown('a'))
	{
		orient *= ang_vel;
		turr.m_stalker.BaseTurns();
	}
	if(SciGame::game->inputter->KeyIsDown('d'))
	{
		orient *= ang_vel.Transpose();
		ang_vel.Transpose();
		turr.m_stalker.BaseTurns();
	}

	SciVector2 forw_vel(0.0f, 83.33333f*dt);
	SciVector2 back_vel(0.0f, 44.44444f*dt);
	forw_vel = orient * forw_vel; // Rotate velocity vector by orientation
	back_vel = orient * back_vel;
	if(SciGame::game->inputter->KeyIsDown('w'))
	{
		pos += forw_vel;
	}
	if(SciGame::game->inputter->KeyIsDown('s'))
	{
		pos -= back_vel;
	}

	hier.Update();
	turr.Update(dt);

	RotateSprite();
	// Sprite rot can be done in update of object it its after hier update
	turr.RotateSprite(turr.sprite, turr.w_orient);
}

void Tank::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(pos);
	SciGame::game->renderer->Draw(rot_sprite, alpha);
	SciGame::game->renderer->ResetWorld();

	turr.Render(alpha);
}
//////// End of Tank ////////

// Clever trick: implement base class' static function
// to let it hold the static pointer to this game
#ifdef TESTGAME
bool SciGame::Create()
{
	SciGame::game = new TestGame;
	return (SciGame::game != nullptr);
}
#endif

// Constructors
TestGame::TestGame()
{
	// Precalculate rotation matrices
	ang_vel.CalcRotation(da);

	// Load dummy sprite...
	fog.LoadFile("img/fog.tga", 90, 40);
	fog_patrol = true;
}
// End of Constructors

// Virtual interface functions
bool TestGame::Initialize(ScreenRes scr)
{
	renderer = new SciRenderer2D(scr);
	inputter = new SciInputModule;
	collisioner = new SciCollision2D;

	yuki = Onna(collisioner->GetCollisions(), 2);
	horo = OnnaNPC(collisioner->GetCollisions(), 0);

	collisioner->AddObject(&yuki);
	collisioner->AddObject(&horo);

	// Load dummy background...
	SciResourceNan<SciPixel> backgrnd;
	backgrnd.LoadFile("img/back.tga", 350, 350);
	renderer->SetBackground(backgrnd);
	return true;
}

void TestGame::Update(float dt)
{
	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	if(!debug_on)
	{
		debug = CreateWindowA("STATIC", "Debug Info", WS_VISIBLE | WS_POPUP,
			100, 0, 400, 400, NULL, NULL, NULL, NULL);
		d_info = CreateWindowA("STATIC", "Debug Info", WS_VISIBLE | WS_CHILD,
			0, 0, 390, 390, debug, NULL, NULL, NULL);
		debug_on = true;
	}

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

	// Detect Collisions
	collisioner->DetectCollisions();

	// Update Onna Senpai and Horo Senpai
	yuki.Update();
	horo.Update();

	// Update PzKpfw Tiger
	tiger.Update(dt);

	// Fog's stuff
	if(fog_patrol && fog_trans.x < renderer->CurScrSize()->w - fog.Width()/2)
	{
		fog_trans.x += 0.4f;
	}
	else
	{
		fog_patrol = false;
	}

	if(!fog_patrol && fog_trans.x > 0.0f)
	{
		fog_trans.x -= 0.4f;
	}
	else
	{
		fog_patrol = true;
	}
}

void TestGame::Render(float alpha)
{
	renderer->ClearBuffer();

	yuki.Render(alpha);
	horo.Render(alpha);
	tiger.Render(alpha);

	renderer->TranslateWorld(fog_trans);
	renderer->Draw(fog, alpha);
	renderer->ResetWorld();

	/*if(inputter->MouseDown(Left))
	{
	SciVector2 mouse_pos = inputter->GetMousePos();
	mouse_pos += SciVector2(1.0f, 1.0f);
	SciSprite tmp(30, 30);
	tmp.Fill(red_color);
	renderer->TranslateWorld(mouse_pos);
	renderer->Draw(tmp, alpha);
	renderer->ResetWorld();
	}*/

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	drawVecDebug(dirg*30.0f, w_posg);
	drawVecDebug(mouseg, w_posg, green_color);

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

	renderer->DrawBuffer();
}
// End of Virtual interface functions