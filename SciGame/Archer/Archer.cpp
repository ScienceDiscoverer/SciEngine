#include <SciGame.h>
#include "Archer.h"
#include "Environment.h"
////////////////// DEBUG INCLUDES ///////////////////
//#define SCI_DEBUG
#include <SciDebug.h>
#ifdef SCI_DEBUG
float tot_ang;
SciVector2 dirg, w_posg, mouseg;
#endif /* SCI_DEBUG */
/////////////////// DEBUG INCLUDES ///////////////////
float g_drag = 0.004f;
float g_lift = 0.0018f;

// Global constants
const SciSprite global_arrow_spr("img/onnabushi/arrow.tga", 25, 3);
//const SciSprite global_arrow_spr("img/box.tga", 30, 30);
//const SciSprite global_arrow_spr("img/arrow_prot.tga", 30, 8);

// Component classes
// LeftHand
// Constructors
LeftHand::LeftHand() : w_pos(zero_vec2)
{
	sprite.LoadFile("img/onnabushi/l_arm.tga", 25, 3);
	SetRotSprite();
	m_stalker.Link(&orient, &w_orient, &w_pos);
	m_stalker.ToggleInitOrient();
	m_stalker.MaxAngVel() = 0.8f;
}
// End of Constructors

// Interface functions
void LeftHand::Update(float dt)
{
	m_stalker.Follow(dt);
	RotateSprite(sprite, w_orient);
}
void LeftHand::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(w_pos);
	SciGame::game->renderer->Draw(rot_sprite, alpha);
	SciGame::game->renderer->ResetWorld();
}
// End of Interface functions
// End of LeftHand

// RightHand
// Constructors
RightHand::RightHand() : w_pos(zero_vec2)
{
	sprite.LoadFile("img/onnabushi/r_arm.tga", 17, 7);
	SetRotSprite();
}
// End of Constructors

// Interface functions
void RightHand::Update(float dt)
{
	RotateSprite(sprite, w_orient);
}
void RightHand::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(w_pos);
	SciGame::game->renderer->Draw(rot_sprite, alpha);
	SciGame::game->renderer->ResetWorld();
}
// End of Interface functions
// End of RightHand

// Arrow
// Constructors
Arrow::Arrow() : w_pos(zero_vec2), cent_disp(-21.0f, 0.0f), in_bow(true)
{
	ex_sprite = &global_arrow_spr;
	SetRotSprite(global_arrow_spr, cent_disp, orient);
	mass = 0.07f;
	//mass = 0.15f;
	//CalcMomInertInv(global_arrow_spr); // MAY BE NOT CORRECT TIP MUST BE HEAVYER!!
	mom_inert_inv = 1.0f / 3.1783333f;
	elast = 0.82f;
	// Aerodynamic properties
	c_pres.Set(-5.0f, 0.0f); // Fletching moves center of farther pressure backwards
	//c_pres.Set(-8.5f, 0.0f); // Fletching moves center of farther pressure backwards
	drag_c = 0.004f;
	lift_c = 0.0018f;
	real_w = 19.0f;
	real_h = 3.0f;
}
// End of Constructors

// Interface functions
void Arrow::Update(float dt)
{
	if(in_bow)
	{
		if(cent_disp.x > -21.0f)
		{
			SetRotSprite(global_arrow_spr, cent_disp, w_orient);
		}
		RotateSprite(*ex_sprite, w_orient, cent_disp);
	}
	else
	{
		const SciArray<PhyCollision>& cols = SciGame::game->physicist->GetPhyCollisions();

		for(int i = 0; i < cols.Size(); ++i)
		{
			PhyEntPtr collider;
			if(cols[i].a == this)
			{
				collider = cols[i].b;
			}
			else if(cols[i].b == this)
			{
				collider = cols[i].a;
			}
			else
			{
				continue;
			}

			Platform* earth = dynamic_cast<Platform*>(collider);
			if(earth)
			{
				const float v_mag = vel.Mag();
				if(v_mag * SciGame::game->physicist->WorldScale() > 1.39f)
				{
					const SciVector2 vel_norm = vel / v_mag;
					if(orient.GetColumn(0).Dot(vel_norm) >= 0.9848f) // Angle between < 10 deg
					{
						is_penning = true;
					}
				}
			}
		}

		/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

		const float mps = vel.Mag()/SciGame::game->physicist->WorldScale();
		out += str("ArrowSpd: ") + tstr(mps) + str(" m/s\n");
		out += str("ArrowSpd: ") + tstr(mps * 3600.0f / 1000.0f) + str(" km/h\n");

#endif /* SCI_DEBUG */
		/////////////////////////////////// DEBUGING //////////////////////////////////


		RotateSprite(*ex_sprite);
	}
}
void Arrow::Render(float alpha)
{
	if(in_bow)
	{
		SciGame::game->renderer->TranslateWorld(w_pos);
	}
	else
	{
		SciGame::game->renderer->TranslateWorld(pos);
	}
	SciGame::game->renderer->Draw(rot_sprite, alpha);
	SciGame::game->renderer->ResetWorld();

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	drawAABRdebug(b_rec);
	//out += str("brecside: ") + tstr(b_rec.Max().x - b_rec.Min().x) + nl;

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////
}
// End of Interface functions

// Private functions
void Arrow::Penetrate()
{

}
// End of Private functions
// End of Arrow
// End of Component classes

// Bow
// Constructors
Bow::Bow() : w_pos(zero_vec2), ya(nullptr), draw(0.0f),
last_draw(0.0f), relese_draw(0.0f), draw_spd(4.5f),
release_spd(50.0f), w_yas(nullptr)
{
	sprite.LoadFile("img/onnabushi/bow.tga", 27, 27);
	SetRotSprite();
}
// End of Constructors

// Interface functions
void Bow::Update(float dt)
{
	if(SciGame::game->inputter->MouseDown(Left) && draw < 9.0f)
	{
		const float draw_pf = draw_spd * dt;
		last_draw = draw;
		draw += draw_pf;
		ya->cent_disp.x += draw_pf;
	}
	else if(draw > 9.0f)
	{
		ya->cent_disp.x -= draw - 9.0f;
		last_draw = draw;
		draw = 9.0f;
	}
	else if(draw > 0.0f && SciGame::game->inputter->MouseUp(Left))
	{
		release_spd += 20.0f;
		const float rel_pf = release_spd * dt;
		last_draw = draw;
		if(draw > relese_draw)
		{
			relese_draw = draw;
		}
		draw -= rel_pf;
		ya->cent_disp.x -= rel_pf;
	}
	else if(SciGame::game->inputter->MouseUp(Left) && last_draw > 0.0f)
	{
		ya->cent_disp.x = -21.0f;
		ReleaseArrow();
		draw = 0.0f;
		last_draw = 0.0f;
		relese_draw = 0.0f;
		release_spd = 50.0f;
	}
	
	RotateSprite(sprite, w_orient);
}
void Bow::Render(float alpha)
{
	SciGame::game->renderer->TranslateWorld(w_pos);
	SciGame::game->renderer->Draw(rot_sprite, alpha);
	SciGame::game->renderer->ResetWorld();
}
// End of Interface functions

// Core operations
void Bow::ReleaseArrow()
{
	//hier.RemoveNode(4);
	//Arrow tmp;
	w_yas->Push(*ya);
	Arrow& new_ya = w_yas->PeekBack();
	new_ya.vel = ya->w_orient.GetColumn(0) * 100.0f * relese_draw * 0.7f; // Impulse
	new_ya.orient = ya->w_orient;
	new_ya.pos = ya->w_pos - new_ya.orient.GetColumn(0) * ya->cent_disp.x;
	new_ya.SetRotSprite(global_arrow_spr);
	new_ya.b_rec.CreateFromSprite(new_ya.rot_sprite);
	new_ya.b_rec.Update(new_ya.pos);
	new_ya.in_bow = false;

	new_ya.drag_c = g_drag;
	new_ya.lift_c = g_lift;

	SciGame::game->collisioner->AddObject(&new_ya);
	SciGame::game->physicist->AddObject(&new_ya);
}
// End of Core operations
// End of Bow

// Main class
// Constructors
Archer::Archer()
{
	sprite.LoadFile("img/onnabushi/body.tga", 19, 31);
	SetRotSprite();
	mass = 65.0f;
	CalcMomInertInv(sprite);
	b_rec.CreateFromSprite(rot_sprite);
	pos.Set(100.0f, 100.0f);
	b_rec.Update(pos);
	elast = 0.55f;
	rot_disabled = true;

	// Build Hierarchy
	Transforms tmp;
	tmp.l_orient = tmp.w_orient = &orient;
	tmp.l_pos = tmp.w_pos = &pos;
	hier.AddNode(-1, tmp);
	tmp.l_orient = &l_hand.orient;
	tmp.w_orient = &l_hand.w_orient;
	tmp.l_pos = &l_hand.pos;
	tmp.w_pos = &l_hand.w_pos;
	hier.AddNode(0, tmp);
	tmp.l_orient = &r_hand.orient;
	tmp.w_orient = &r_hand.w_orient;
	tmp.l_pos = &r_hand.pos;
	tmp.w_pos = &r_hand.w_pos;
	hier.AddNode(1, tmp);
	tmp.l_orient = &yumi.orient;
	tmp.w_orient = &yumi.w_orient;
	tmp.l_pos = &yumi.pos;
	tmp.w_pos = &yumi.w_pos;
	hier.AddNode(1, tmp);
	tmp.l_orient = &ya.orient;
	tmp.w_orient = &ya.w_orient;
	tmp.l_pos = &ya.pos;
	tmp.w_pos = &ya.w_pos;
	hier.AddNode(3, tmp);

	// Put arrow in the bow
	yumi.InsertArrow(&ya);
}
// End of Constructors

// Interface functions
void Archer::Update(float dt)
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
	
	SciVector2 c_force(zero_vec2); // Control Force
	if(SciGame::game->inputter->KeyIsDown('i'))
	{
		c_force += SciVector2(0.0f, 12000.0f);
	}
	if(SciGame::game->inputter->KeyIsDown('k'))
	{
		c_force += SciVector2(0.0f, -12000.0f);
	}
	if(SciGame::game->inputter->KeyIsDown('j'))
	{
		c_force += SciVector2(-12000.0f, 0.0f);
	}
	if(SciGame::game->inputter->KeyIsDown('l'))
	{
		c_force += SciVector2(12000.0f, 0.0f);
	}
	forces = c_force;

	if(SciGame::game->inputter->KeyIsDown('w'))
	{
		g_drag += 0.0001f;
	}
	if(SciGame::game->inputter->KeyIsDown('s'))
	{
		g_drag -= 0.0001f;;
	}
	if(SciGame::game->inputter->KeyIsDown('e'))
	{
		g_lift += 0.0001f;
	}
	if(SciGame::game->inputter->KeyIsDown('d'))
	{
		g_lift -= 0.0001f;;
	}

	hier.Update();
	l_hand.Update(dt);
	r_hand.Update();
	yumi.Update(dt);
	ya.Update();

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	out += str("Drag: ") + tstr(g_drag) + nl;
	out += str("Lift: ") + tstr(g_lift) + nl;

	SetWindowTextA(d_info, out.c_str());
	out.clear();

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////
}
void Archer::Render(float alpha)
{
	l_hand.Render(alpha);

	SciGame::game->renderer->TranslateWorld(pos);
	SciGame::game->renderer->Draw(rot_sprite, alpha);
	SciGame::game->renderer->ResetWorld();

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	drawAABRdebug(b_rec, red_color);

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

	yumi.Render(alpha);
	ya.Render(alpha);
	r_hand.Render(alpha);
}
// End of Interface functions
// End of Main class