#include "SciControls.h"
#include "SciMath/SciMath.h"
#include "SciGame.h"

// Constructors
MouseTracker::MouseTracker() : orient(nullptr), w_orient(nullptr), w_pos(nullptr),
	max_ang_vel(DEG_RAD(12.0f)), rot_num(-1), old_mouse(zero_vec2),
	base_rotates(false), delta_t(-1.0f), init_or(1) {}
// End of Constructors

// Access functions
void MouseTracker::Link(SciMatrix2x2* ori, const SciMatrix2x2* w_ori, const SciVector2* w_p)
{
	orient = ori;
	w_orient = w_ori;
	w_pos = w_p;
}
// End of Access functions

// Core operations
void MouseTracker::Follow(float dt)
{
	if(dt != delta_t)
	{
		delta_t = dt;
		ang_pf = max_ang_vel * dt;
		rot_pf.CalcRotation(ang_pf);
	}

	// Compute normilized vector pointing to mouse cursor
	SciVector2 mouse = (SciVector2)SciGame::game->inputter->GetMousePos() - *w_pos;

	if(mouse != old_mouse || base_rotates)
	{
		old_mouse = mouse;
		mouse.Normalize();

		// Get j (or i) vector of orientation as direction vector
		const SciVector2 dir(w_orient->GetColumn(init_or));

		const float dir_dot_m = dir.Dot(mouse);
		const float total_ang = sciArcCos(dir_dot_m);
		const float times_rot = total_ang / ang_pf;
		float int_times;
		leftover = sciIntFrac(times_rot, int_times) * ang_pf;
		sciCleanF(leftover);
		rot_num = (int)int_times;
		// Rotate dir vector c.clock. to determine in what direction to turn
		const float ndir_dot_m = (rot_1_deg * dir).Dot(mouse);
		if(ndir_dot_m < dir_dot_m)
		{
			rot_pf.MakeClock();
			leftover = -leftover;
		}
		else
		{
			rot_pf.MakeCountClock();
		}
	}

	// Rotate turret every frame, until it reaches the mouse
	if(rot_num > 0)
	{
		*orient *= rot_pf;
		--rot_num;
	}
	else if(!rot_num) // Rotate turret by leftover angle
	{
		SciMatrix2x2 leftover_rot;
		leftover_rot.CalcRotation(leftover);
		*orient *= leftover_rot;
		--rot_num;
	}
}
// End of Core operations