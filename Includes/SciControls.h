#ifndef SCICONTROLS_H
#define SCICONTROLS_H

#include "SciMath/SciVector2.h"
#include "SciMath/SciMatrix2x2.h"

// Mouse Tracker that interpolates orientation based on current mouse
// position. Supports object hierarchies.
class MouseTracker
{
public:
	// Constructors
	MouseTracker();

	// Access functions
	void Link(SciMatrix2x2* ori, const SciMatrix2x2* w_ori, const SciVector2* w_p);
	float& MaxAngVel() { return max_ang_vel; }
	void BaseTurns() { base_rotates = true; }
	void BaseStill() { base_rotates = false; }
	void ToggleInitOrient() { init_or ? init_or = 0 : init_or = 1; }

	// Core operations
	void Follow(float dt);

private:
	// Object's state data
	SciMatrix2x2* orient; // Local orientation
	const SciMatrix2x2* w_orient; // World orientation
	const SciVector2* w_pos; // World position
	// Vital data
	float max_ang_vel; // Maximum rotation speed
	float ang_pf; // Angle of per-frame rotation
	SciMatrix2x2 rot_pf; // Per-frame rotation
	int rot_num; // Num. of perframe rot. to perform to reach mouse
	float leftover; // Leftover angle after rotating rot_num times
	// Misc data
	SciVector2 old_mouse; // Last frame's mouse vector
	bool base_rotates; // Base object is rotating in this frame
	float delta_t;
	int init_or; // Initial orientation (up/right)
};
// Note: More simple (but too instant) approach to this will be just this:
/*SciVector2 norm = (SciVector2)SciGame::game->inputter->GetMousePos() - w_pos;
norm.Normalize();
orient.SetColumns(-norm.Perp(), norm);*/

#endif /* SCICONTROLS_H */
