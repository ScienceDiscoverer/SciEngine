#include <limits>
#include "SciCollision2D.h"
#include "SciMath/SciMath.h"
////////////////// DEBUG INCLUDES ///////////////////
//#define SCI_DEBUG
#ifdef SCI_DEBUG
#include <windows.h>
#include <string>
#include <fstream>
#include "SciGame.h"
#undef min
#undef max
typedef std::string str;

HWND debug, d_info;
bool debug_on = false;
int frames = 0;
str out;
const str nl("\n");

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
void drawAABRdebug(SciAABR box, const SciPixel& col = blue_color)
{
	const SciVector2 vec1(0.0f, box.Max().y - box.Min().y);
	const SciVector2 vec2(box.Max().x - box.Min().x, 0.0f);
	const SciVector2 vec3 = box.Max() - box.Min() - vec1;
	const SciVector2 vec4 = box.Max() - box.Min() - vec2;
	drawVecDebug(vec1, box.Min(), col);
	drawVecDebug(vec2, box.Min(), col);
	drawVecDebug(-vec3, box.Max(), col);
	drawVecDebug(-vec4, box.Max(), col);
}
float tot_ang;
SciVector2 dirg, w_posg, mouseg;
#endif /* SCI_DEBUG */
/////////////////// DEBUG INCLUDES ///////////////////

// Typedefs
typedef std::numeric_limits<float> Infinity;

// Interface functions
void SciCollision2D::AddObject(ColEntPtr obj)
{
	// Add Sweep-and-Prune data
	Endpoint tmp;
	// Calculate low endpoint
	tmp.extent = obj->b_rec.Min().x;
	tmp.low = true;
	tmp.obj = obj;
	extents.Push(tmp);
	// Calculate high endpoint
	tmp.extent = obj->b_rec.Max().x;
	tmp.low = false;
	extents.Push(tmp);
}

void SciCollision2D::EliminateObject(ColEntPtr obj)
{
	for(int i = 0; i < extents.Size(); ++i)
	{
		if(extents[i].obj == obj)
		{
			extents.Pop(i);
		}
	}
}

const SciArray<Collision>& SciCollision2D::DetectCollisions()
{
	UpdateSweep();
	extents.Sort();
	collisions.Clean();

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

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	out += tstr(extents.Size()) + nl;
	out += tstr(collisions.Size()) + nl;
	for(int i = 0; i < collisions.Size(); ++i)
	{
		out += tstr((int)collisions[i].a) + str(" | ") + tstr((int)collisions[i].b) + nl;
		out += tstr(collisions[i].info.pen) + nl;
	}

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

	// Detect collisions
	// Important note: sweep&prune works in a way, that objects with lower min x-extent
	// will always be "object b" in DetectPixelCollisions(a, b) function!
	// Reason: objects with higher extent always get checked with lower ones, that are already in system
	for(int i = 0; i < extents.Size(); ++i)
	{
		ColEntPtr obj = extents[i].obj; // extent's object
		if(extents[i].low)
		{
			// Check collisions with all potential colliders
			if(obj->is_moving) // Object is moving, check all
			{
				for(int j = 0; j < pos_colliders.Size(); ++j)
				{
					ColEntPtr pc = pos_colliders[j]; // Possible collider
					if(obj->b_rec.IsColliding(pc->b_rec))
					{
						//AddCollision(obj, pc);
						DetectPixelCollisions(obj, pc);
					}
				}
			}
			else // Object is not moving, check only against moving ones
			{
				for(int j = 0; j < pos_colliders.Size(); ++j)
				{
					ColEntPtr pc = pos_colliders[j]; // Possible collider
					if(pc->is_moving && obj->b_rec.IsColliding(pc->b_rec))
					{
						//AddCollision(obj, pc);
						DetectPixelCollisions(obj, pc);
					}
				}
			}

			// Add object to possible colliders list
			pos_colliders.Push(obj);
		}
		else
		{
			// Max extent reached, this object can't collide anymore
			pos_colliders.Remove(obj);
		}
	}

	return collisions;
}
// End of Interface functions

// Private functions
// Update Sweep-and-Prune data
void SciCollision2D::UpdateSweep()
{
	for(int i = 0; i < extents.Size(); ++i)
	{
		const SciAABR rect = extents[i].obj->b_rec;
		if(extents[i].low)
		{
			extents[i].extent = rect.Min().x;
		}
		else
		{
			extents[i].extent = rect.Max().x;
		}
	}
}

// Apply per-pixel collision detection routine
void SciCollision2D::DetectPixelCollisions(ColEntPtr a, ColEntPtr b)
{
	// Get detailed bounding rectangles collision information 
	CollisionAABR rcol = a->b_rec.GetFullCollision(b->b_rec);

	SciArray<SciAABR> sbr_a; // Scanline Bounding Rectangles for pixels of sprite a
	ConstructSBRs(sbr_a, rcol, a);

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	out += str("SBR a size:") + tstr(sbr_a.Size()) + nl;

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

	if(!sbr_a.Size())
	{
		return;
	}

	SciArray<SciAABR> sbr_b; // Scanline Bounding Rectangles for pixels of sprite b
	ConstructSBRs(sbr_b, rcol, b);

	// Check for intersections between SBRs
	SciVector2 min(Infinity::max(), Infinity::max());
	SciVector2 max(Infinity::min(), Infinity::min());
	bool col_detected = false;

	for(int i = 0; i < sbr_a.Size(); ++i)
	{
		for(int j = 0; j < sbr_b.Size(); ++j)
		{
			if(sbr_a[i].IsColliding(sbr_b[j]))
			{
				col_detected = true;
				SciAABR tmp = sbr_a[i].GetCollisionRect(sbr_b[j]);
				if(tmp.Min().x < min.x)
				{
					min.x = tmp.Min().x;
				}
				if(tmp.Min().y < min.y)
				{
					min.y = tmp.Min().y;
				}
				if(tmp.Max().x > max.x)
				{
					max.x = tmp.Max().x;
				}
				if(tmp.Max().y > max.y)
				{
					max.y = tmp.Max().y;
				}
			}
		}
	}

	if(!col_detected)
	{
		return;
	}

	const float pen_x = max.x - min.x;
	const float pen_y = max.y - min.y;
	const SciVector2 centroid(pen_x/2.0f, pen_y/2.0f);

	Collision col;
	col.a = a;
	col.b = b;
	col.info.normal = rcol.info.normal;

	if(pen_x < pen_y)
	{
		col.info.pen = pen_x;
	}
	else
	{
		col.info.pen = pen_y;
	}

	col.info.point = min + centroid;

	collisions.Push(col);
}

// Calculate collision normal and penetration depth, add to collisions array (between AABRs)
void SciCollision2D::AddCollision(ColEntPtr a, ColEntPtr b)
{
	CollisionAABR rcol = a->b_rec.GetFullCollision(b->b_rec);

	// Fill up Collision entry
	Collision col;
	col.a = a;
	col.b = b;
	col.info = rcol.info;

	collisions.Push(col);
}

const SciSprite& SciCollision2D::GetSprite(ColEntPtr obj)
{
	if(obj->rot_sprite.Size())
	{
		return obj->rot_sprite;
	}
	
	if(obj->ex_sprite)
	{
		return *obj->ex_sprite;
	}

	return obj->sprite;
}

// Construct Scanline Bounding Rectangles (SBR) from sprite fragment
void SciCollision2D::ConstructSBRs(SciArray<SciAABR>& arr, const CollisionAABR& rc, ColEntPtr obj)
{
	// Choose what sprite to use
	const SciSprite& spr = GetSprite(obj);
	const SciVector2& min = obj->b_rec.Min();

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	out += str("RCmin: ") + tstr(rc.min) + nl;
	out += str("SpriteMin: ") + tstr(min) + nl;

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

	const int h = sciFloorI(rc.pen_y);
	const int w = sciFloorI(rc.pen_x);
	
	// Construct Scanline Bounding Rectengles (SBR) of object's pixels
	bool last_solid = false; // Previous pixel was solid
	for(int i = 0; i < h; ++i)
	{
		SciAABR rect;
		for(int j = 0; j < w; ++j)
		{
			// Map collision box min (origin) to sprite's min (origin)
			const SciVector2 map_m = rc.min - min;
			const SciVector2 pix = map_m + SciVector2((float)j, (float)i);
			// Find real local pixel position in sprite space
			const SciVector2 real_pix(sciFloor(pix.x), sciFloor(pix.y));
			if((int)real_pix.x >= spr.Width() || (int)real_pix.y >= spr.Height())
			{
				continue;
			}
			// Translate min to find correct pixel world coordinates
			const SciVector2 pix_w = min + real_pix;

			// Check if pixel at this coordinates is solid
			if(spr((int)real_pix.x, (int)real_pix.y).a)
			{
				const SciVector2 pw_max = pix_w + SciVector2(1.0f, 1.0f); // Max extent of pixel
				if(!last_solid) // Start new SBR
				{
					rect.SetMin(pix_w);
					rect.SetMax(pw_max);
				}
				else // Extend current SBR max extent
				{
					rect.SetMax(pw_max);
				}

				last_solid = true;
			}
			else if(last_solid) // End SBR and add it to array
			{
				arr.Push(rect);
				last_solid = false;
			}
		}

		if(last_solid)
		{
			arr.Push(rect);
			last_solid = false;
		}
	}
}
// End of Private functions