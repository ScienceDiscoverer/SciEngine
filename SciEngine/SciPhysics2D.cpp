#include "SciPhysics2D.h"
#include "SciMath/SciMath.h"
////////////////// DEBUG INCLUDES ///////////////////
//#define SCI_DEBUG
#ifdef SCI_DEBUG
#include <windows.h>
#include <string>
#include <fstream>
#include "SciGame.h"
//MessageBox(NULL, "back not loded", "Error", MB_OK | MB_ICONERROR);
HWND debug, d_info;
bool debug_on = false;
typedef std::string str;
template <class T>
str tstr(T srs) { return std::to_string(srs); }
template <>
str tstr(SciVector2 srs) { return str("[")+std::to_string(srs.x)+str("|")+std::to_string(srs.y)+str("]"); }
int frames = 0;
str out;
#endif /* SCI_DEBUG */
/////////////////// DEBUG INCLUDES ///////////////////

// Constants
// All forces need to be scaled by how much pixels is 1 m in a game world
const SciVector2 std_earth_g(0.0f, -9.80665f);
// Number of pixels in 1 real world meter
const float irl_m = 3703.7037f;
const float earth_air_dens = 1.1839f; // Earth air, 25degC, 1atm

// Helper functions
// Find linear velocity of a point (r dist from cent. of mass) from angular velocity
// Analogous to ang_v.Cross(r) in 3D world
SciVector2 angLin(const SciVector2& r, const float ang_v)
{
	SciVector2 vel;
	vel.x = -r.y * ang_v;
	vel.y = r.x * ang_v;
	return vel;
}
// End of Helper functions

// Interface functions
// Global forces will be applied to all objects in the system in all frames
// Note: all global forces must be provided for bodies with 1 kg mass
void SciPhysics2D::AddGlobalForce(GlobalForce gforce, const SciVector2& force)
{
	switch(gforce)
	{
	case Custom:
		g_forces += force;
		break;

	case Gravity:
		g_forces += std_earth_g * world_scale;
		break;

	default:
		break;
	}
}

void SciPhysics2D::RemoveGlobalForce(GlobalForce gforce, const SciVector2& force)
{
	switch(gforce)
	{
	case Custom:
		g_forces -= force;
		break;

	case Gravity:
		g_forces -= std_earth_g * world_scale;
		break;

	default:
		break;
	}
}

float SciPhysics2D::ConfigAir(bool enabled, float air_den)
{ 
	air_enabled = enabled;
	air_dens = air_den / world_scale;
	return air_dens;
}

// All the Physics Magic happens here
void SciPhysics2D::Integrate(float dt)
{
	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	out += str("<--------------| Frame ") + tstr(frames) + (" |-------------->") + str("\n");

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////
	
	CollisionResponse();

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

	// Integrate every object in the Physics Engine
	for(int i = 0; i < objs.Size(); ++i)
	{
		// Skip immovable objects
		if(!objs[i]->movable)
		{
			continue;
		}

		const float m = objs[i]->mass;
		float& ang_mom = objs[i]->ang_mom;
		float& ang_vel = objs[i]->ang_vel;
		SciMatrix2x2& orient = objs[i]->orient;
		SciVector2& vel = objs[i]->vel;
		SciVector2& lf_accel = objs[i]->last_frame_accel;
		SciVector2& pos = objs[i]->pos;

		// Fluid dynamics
		SciVector2 aero_force = zero_vec2; // Aerodynamic force
		if(air_enabled)
		{
			/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

			out += str("vel: ") + tstr(vel) + str("\n");
			//SetWindowTextA(d_info, out.c_str());

#endif /* SCI_DEBUG */
			/////////////////////////////////// DEBUGING //////////////////////////////////
			
			
			const float v_mag_sqr = vel.MagSqr();
			// Compute object's sides
			const SciVector2 io = orient.GetColumn(0);
			const SciVector2 jo = orient.GetColumn(1);
			const SciVector2 r_width = io * objs[i]->real_w;
			const SciVector2 r_height = jo * objs[i]->real_h;
			// Find perp. component of sides on velocity (this will give reference length affected by air)
			const float w_dot_v = r_width.Dot(vel);
			const float h_dot_v = r_height.Dot(vel);
			// Compute width and height reference vector (perp. to air flow)
			float ref_len = 0.0f, w_ref_len = 0.0f, h_ref_len = 0.0f;
			if(v_mag_sqr)
			{
				const float vmags = vel.MagSqr();
				const SciVector2 w_ref = r_width - w_dot_v / vmags * vel;
				const SciVector2 h_ref = r_height - h_dot_v / vmags * vel;
				// Calculate width/height reference lengths and total reference length
				const float w_ref_len = w_ref.Mag();
				const float h_ref_len = h_ref.Mag();
				ref_len = w_ref_len + h_ref_len;
			}
			
			// Compute drag component of total aerodynamic force
			const float drag_mag = objs[i]->drag_c * 0.5f * air_dens * v_mag_sqr * ref_len;
			const SciVector2 drag = -vel.Normalized() * drag_mag;

			// Find correct lift normal, for given speed vector and orientation
			SciVector2 surf_norm; // Surface normal for lift force
			if(w_dot_v < 0.0f)
			{
				if(h_ref_len > w_ref_len)
				{
					surf_norm = io;
				}
				else
				{
					if(h_dot_v < 0.0f)
					{
						surf_norm = jo;
					}
					else
					{
						surf_norm = -jo;
					}
				}
			}
			else
			{
				if(h_ref_len > w_ref_len)
				{
					surf_norm = -io;
				}
				else
				{
					if(h_dot_v < 0.0f)
					{
						surf_norm = jo;
					}
					else
					{
						surf_norm = -jo;
					}
				}
			}

			// Compute lift component of total aerodynamic force
			const float lift_mag = objs[i]->lift_c * 0.5f * air_dens * v_mag_sqr * ref_len;
			const SciVector2 lift = surf_norm * lift_mag;

			aero_force = lift + drag;
			aero_force.Clean();

			/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

			out += str("Lift_Mag: ") + tstr(lift_mag) + str("\n");
			out += str("Drag_Mag: ") + tstr(drag_mag) + str("\n");
			out += str("objs[i]->lift_c: ") + tstr(objs[i]->lift_c) + str("\n");
			out += str("air_dens: ") + tstr(air_dens) + str("\n");
			out += str("v_mag_sqr: ") + tstr(v_mag_sqr) + str("\n");
			out += str("ref_len: ") + tstr(ref_len) + str("\n");

			//SetWindowTextA(d_info, out.c_str());

#endif /* SCI_DEBUG */
			/////////////////////////////////// DEBUGING //////////////////////////////////
		}
		
		// Linear dynamics
		// Compute total force that is applied upon object
		// CHECK IF ALL AEROFORCE IS APPLYIED AS LINEAR COMP OR NEED TO CALC IT!!!!!!!!!!!!!!!!!!!!!!!!!
		const SciVector2 t_force = objs[i]->forces + g_forces * m + aero_force;
		SciVector2 accel = t_force / m;
		accel.Clean();

		// Remember acceleration for this frame
		lf_accel = dt * accel;

		// Integrate using semi-implicit (symplectic) Euler integrator
		// Velocity in next frame (future velocity) - Explicit Euler
		vel += lf_accel;
		// Clean very small values to improve performance and avoid precision bugs
		vel.Clean();

		// Position in current frame, calculated using future velocity - Implicit Euler
		const SciVector2 t = dt * vel;
		pos += t;
		// Update object's bounding object
		objs[i]->UpdateBound(t);

		// Rotational dynamics
		// Generate some fake angular drag - maybe it is not the best idea?
		float ang_drag;
		if(sciAbs(ang_vel) < 0.1f) // Apply huge drag on low speeds to counteract micro rotations
		{
			ang_drag = ang_vel * 1000.0f;
		}
		else
		{
			ang_drag = ang_vel * 0.45f;
		}

		// Compute torque from aerodynamic force
		float aero_torq = 0.0f;
		if(air_enabled)
		{
			// Get world space center of pressure
			const SciVector2 c_press = orient * objs[i]->c_pres + pos;
			const SciVector2 r = c_press - pos;
			// Calculate aerodynamic torque
			aero_torq = r.PerpDot(aero_force);
			sciCleanF(aero_torq);
		}

		// Integrate Torque to compute new Angular Momentum
		ang_mom += dt * (objs[i]->torqs - ang_drag + aero_torq);
		sciCleanF(ang_mom);

		// Update Angular Velocity
		ang_vel = objs[i]->mom_inert_inv * ang_mom;
		sciCleanF(ang_vel);
		// Convert Angular Velocity into linear speed at orient base vectors tips
		SciVector2 v1 = orient.GetColumn(0);
		SciVector2 v2 = orient.GetColumn(1);
		v1 += dt * angLin(v1, ang_vel);
		v2 += dt * angLin(v2, ang_vel);
		// Update Orientation
		orient.SetColumns(v1.Normalize(), v2.Normalize());
	}

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	//out.clear();
	++frames;

	if(SciGame::game->inputter->KeyIsDown('s'))
	{
		std::ofstream ofs("debug.txt");
		ofs << out;
		ofs.close();
	}

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

}
// End of Interface functions

// Private functions
void SciPhysics2D::CollisionResponse()
{
	FilterCollisions();
	
	for(int i = 0; i < phy_cols.Size(); ++i)
	//for(int i = phy_cols.Size()-1; i >= 0; --i)
	{
		PhyEntPtr a = phy_cols[i].a; // Object a
		PhyEntPtr b = phy_cols[i].b; // Object b
		const SciVector2 norm = phy_cols[i].info.normal;
		const SciVector2 col_pnt = phy_cols[i].info.point;
		const float ma = a->mass;
		const float mb = b->mass;
		const bool mova = a->movable;
		const bool movb = b->movable;

		// Push out interpenetrating objects
		SciVector2 push_out = phy_cols[i].info.pen * norm;
		if(!mova)
		{
			b->pos += push_out;
			b->b_rec.Update(push_out);
		}
		else if(!movb)
		{
			a->pos -= push_out;
			a->b_rec.Update(-push_out);
		}
		else
		{
			push_out *= 0.5f;
			a->pos -= push_out;
			a->b_rec.Update(-push_out);
			b->pos += push_out;
			b->b_rec.Update(push_out);
		}

		// Calculate reletive velocity
		const SciVector2 r1 = col_pnt - a->pos;
		const SciVector2 r2 = col_pnt - b->pos;
		SciVector2 vel1, vel2;
		// "Cross product" in 2D
		vel1.x = a->vel.x - r1.y * a->ang_vel;
		vel1.y = a->vel.y + r1.x * a->ang_vel;
		vel2.x = b->vel.x - r2.y * b->ang_vel;
		vel2.y = b->vel.y + r2.x * b->ang_vel;
		const SciVector2 rel_v = vel1 - vel2;

		// Handle resting contact micro-collisions
		const float accel_vel = a->last_frame_accel.Dot(norm);
		//const float linear_rv = (a->vel - b->vel).Dot(norm);

		/////////////////////////////////////// DEBUGING //////////////////////////////////
//#ifdef SCI_DEBUG

		//out += str("A is above B: ") + tstr(a->pos.y > b->pos.y) + str("\n");
		//out += str("A pos: ") + tstr(a->pos) + str(" B pos: ") + tstr(b->pos) + str("\n");

//#endif /* SCI_DEBUG */
		/////////////////////////////////////// DEBUGING //////////////////////////////////

		// Add micro-impulse to counteract forces into objects
		a->vel -= accel_vel * norm;

		// Calulate part of reletive velocity applied along collision normal
		float rv_dot_n = rel_v.Dot(norm);
		if(rv_dot_n < 0)
		{
			continue; // Bodies are moving away from each other, no response needed
		}

		/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG1

		/*out += str("VelFromAccel: ") + tstr(a->last_frame_accel.Mag()) + str("\n") + tstr(b->last_frame_accel.Mag())+str("\n");
		out += str("AccelVel: ") + tstr(accel_vel) + str("\n");
		out += str("Vel1: ") + tstr(a->vel.Mag()) + str(" + Vel2: ") + tstr(b->vel.Mag()) + str("\n");
		out += str("RvDotn: ") + tstr(rv_dot_n) + str("\n");
		out += str("RvDotnLinear: ") + tstr((a->vel - b->vel).Dot(norm)) + str("\n");
		out += str("VelLinearMag: ") + tstr((a->vel - b->vel).Mag()) + str("\n");
		out += str("Frames from last collision: ") + tstr(frames) + str("\n");
		out += str("PenDepth: ") + tstr(phy_cols[i].info.pen) + str("\n");*/
		//frames = 0;
		out += str("<-----------| Object #") + tstr(i) + str("|----------->\n");
		out += str("CenterPoint: ") + tstr(a->pos) + str("\n");
		out += str("CollPoint: ") + tstr(phy_cols[i].info.point) + str("\n");

#endif /* SCI_DEBUG */
		/////////////////////////////////// DEBUGING //////////////////////////////////

		// Compute linear factor in denominator
		float denom = 1.0f/ma + 1.0f/mb;
		// Compute rotational factor in denominator
		const float ang_vel1 = a->mom_inert_inv * r1.PerpDot(norm);
		const float ang_vel2 = b->mom_inert_inv * r2.PerpDot(norm);
		const SciVector2 vels_sum = angLin(r1, ang_vel1) + angLin(r2, ang_vel2);
		denom += vels_sum.Dot(norm);

		// Modify reletive velocity by reletive masses of objects and rotational dynamics factors
		const float mod_rvel = rv_dot_n / denom;

		// Restitution must be same for each pair of objects, calc. approximation
		float restitution;
		if((a->vel - b->vel).Mag() < 8.0f)
		{
			restitution = 0.0f; // Lower restitution for very low speeds
		}
		else
		{
			restitution = a->elast * b->elast;
		}
		
		// Compute impulse scale factor
		const float j = (1.0f + restitution) * mod_rvel;

		// Check if object is movable
		if(mova)
		{
			// Update object a linear velocity
			a->vel -= j / ma * norm;
			if(!a->rot_disabled)
			{
				// Update object a angular velocity
				a->ang_mom -= r1.PerpDot(j * norm);
				a->ang_vel = a->mom_inert_inv * a->ang_mom;
			}
			// Generate some friction
			a->vel -= a->vel * 0.025f;
		}

		/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG1

		/*out += str("AVel_after_resol: ") + tstr(a->vel.Mag()) + str("\n");
		out += str("AVel_vec: ") + tstr(a->vel) + str("\n");
		out += str("denom: ") + tstr(denom) + str("\n");
		out += str("vels_sum.Dot(norm): ") + tstr(vels_sum.Dot(norm)) + str("\n");
		out += str("j: ") + tstr(j) + str("\n");*/
		out += str("Restitution: ") + tstr(restitution) + str("\n");
		out += str("ANGVEL: ") + tstr(a->ang_vel) + str("\n");

#endif /* SCI_DEBUG */
		/////////////////////////////////// DEBUGING //////////////////////////////////

		if(movb)
		{
			// Update object b linear velocity
			b->vel += j / mb * norm;
			if(!b->rot_disabled)
			{
				// Update object b angular velocity
				b->ang_mom += r2.PerpDot(j * norm);
				b->ang_vel = b->mom_inert_inv * b->ang_mom;
			}
			// Generate some friction
			b->vel -= b->vel * 0.025f;
		}
	}

	///////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG1

	out += str("Resolved ") + tstr(phy_cols.Size()) + str(" collisions.") + str("\n");
	SetWindowTextA(d_info, out.c_str());
	out.clear();

	/*if(SciGame::game->inputter->KeyIsDown('s'))
	{
		std::ofstream ofs("debug.txt");
		ofs << out;
		ofs.close();
	}*/

#endif /* SCI_DEBUG */
	///////////////////////////////////// DEBUGING //////////////////////////////////
}

// Separate Physical Collisions from General Collisions
void SciPhysics2D::FilterCollisions()
{
	phy_cols.Clean();
	const SciArray<Collision>& cols = *gen_cols;

	for(int i = 0; i < cols.Size(); ++i)
	{
		PhyEntPtr phy_a = dynamic_cast<PhyEntPtr>(cols[i].a);
		PhyEntPtr phy_b = dynamic_cast<PhyEntPtr>(cols[i].b);
		if(!phy_a || !phy_b)
		{
			continue; // One of the obj. or both are non-physical objects, skip collision
		}
		// Objects are valid, compose Physical Collision and add to array
		phy_cols.Push(PhyCollision(phy_a, phy_b, cols[i].info));
	}
}
// End of Private functions