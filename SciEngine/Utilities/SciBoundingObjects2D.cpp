#include "SciBoundingObjects2D.h"
////////////////// DEBUG INCLUDES ///////////////////
//#define SCI_DEBUG
#ifdef SCI_DEBUG
#include <windows.h>
#include <string>
#include <fstream>
#include "../SciGame.h"
typedef std::string str;

HWND debug, d_info;
bool debug_on = false;
int frames = 0;
str out;
str nl("\n");

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

// Bounding Circle
// Collision detection functions
bool SciBCircle::IsColliding(const SciBCircle& other) const
{
	float dist_sqr = DistanceSqr(center_, other.center_);
	float rad_sum = radius_ + other.radius_;
	return dist_sqr <= rad_sum * rad_sum;
}
// End of Collision detection functions
// End of Bounding Circle

// Aix-Aligned Bounding Rectangle
// Access functions
void SciAABR::Update(const SciVector2& trans)
{
	minima += trans;
	maxima += trans;
}

void SciAABR::CreateFromSprite(const SciSprite& spr, const SciVector2& pos)
{
	maxima = SciVector2((float)spr.Width()/2, (float)spr.Height()/2);
	minima = -maxima;
	maxima += pos;
	minima += pos;
}
// End of Access functions

// Collision detection functions
bool SciAABR::IsColliding(const SciAABR& other) const
{
	if(minima.x > other.maxima.x || other.minima.x > maxima.x)
	{
		return false; // Boxes separated on x aix, no collision
	}
	if(minima.y > other.maxima.y || other.minima.y > maxima.y)
	{
		return false; // Boxes separated on y aix, no collision
	}

	return true; // No separation planes where found, objs must collide
}

SciAABR SciAABR::GetCollisionRect(const SciAABR& other) const
{
	const SciVector2 amin = minima;
	const SciVector2 amax = maxima;
	const SciVector2 bmin = other.minima;
	const SciVector2 bmax = other.maxima;
	SciVector2 cmin, cmax; // Collision box extremum (origin)

	// Get collision box extremum (origin)
	if(amin.x <= bmin.x) // A penetrates B from the left
	{
		if(amin.y <= bmin.y) // A penetrates B from the bot
		{
			cmin = bmin;
			if(amax.x > bmax.x) // A is wider than B
			{
				if(amax.y > bmax.y) // A devoured B
				{
					cmax = bmax;
				}
				else
				{
					cmax.Set(bmax.x, amax.y);
				}
			}
			else
			{
				if(amax.y > bmax.y) // A is taller than B
				{
					cmax.Set(amax.x, bmax.y);
				}
				else // Left-Bot corner case
				{
					cmax = amax;
				}
			}
		}
		else // A penetrates B from the top
		{
			cmin.Set(bmin.x, amin.y);
			if(amax.y < bmax.y) // A is shorter than B
			{
				if(amax.x < bmax.x) // A is shorter and thiner than B
				{
					cmax = amax;
				}
				else // A is shorter but wider than B
				{
					cmax.Set(bmax.x, amax.y);
				}
			}
			else
			{
				if(amax.x > bmax.x) // A is wider and taller than B
				{
					cmax = bmax;
				}
				else // Top-Left corner case
				{
					cmax.Set(amax.x, bmax.y);
				}
			}
		}
	}
	else // A penetrates B from the right
	{
		if(amin.y <= bmin.y) // A penetrates B from the bot
		{
			cmin.Set(amin.x, bmin.y);
			if(amax.x < bmax.x) // A is thiner than B
			{
				if(amax.y < bmax.y) // A is thiner and shorter than B
				{
					cmax = amax;
				}
				else // A is thiner but higher than B
				{
					cmax.Set(amax.x, bmax.y);
				}
			}
			else
			{
				if(amax.y > bmax.y) // A is taller than B
				{
					cmax = bmax;
				}
				else // Right-Bot corner case
				{
					cmax.Set(bmax.x, amax.y);
				}
			}
		}
		else // A penetrates B from the top
		{
			cmin = amin;
			if(amax.x < bmax.x) // A is thiner than B
			{
				if(amax.y < bmax.y) // A is inside B
				{
					cmax = amax;
				}
				else
				{
					cmax.Set(amax.x, bmax.y);
				}
			}
			else
			{
				if(amax.y < bmax.y) // A is shorter than B
				{
					cmax.Set(bmax.x, amax.y);
				}
				else // Right-Top corner case
				{
					cmax = bmax;
				}
			}
		}
	}

	return SciAABR(cmin, cmax);
}

CollisionAABR SciAABR::GetFullCollision(const SciAABR& other) const
{
	const SciVector2 amin = minima;
	const SciVector2 amax = maxima;
	const SciVector2 bmin = other.minima;
	const SciVector2 bmax = other.maxima;
	SciVector2 norm_x, norm_y;
	SciVector2 cmin, cmax; // Collision box extremum (origin)

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
	out += str("Amin.x: ") + tstr(amin.x) + str("Bmin.x: ") + tstr(bmin.x) + nl;

#endif /* SCI_DEBUG */
	/////////////////////////////////// DEBUGING //////////////////////////////////

	// Get collision box extremum (origin)
	if(amin.x <= bmin.x) // A penetrates B from the left
	{
		norm_x.Set(1.0f, 0.0f);
		if(amin.y <= bmin.y) // A penetrates B from the bot
		{
			norm_y.Set(0.0f, 1.0f);
			cmin = bmin;
			if(amax.x > bmax.x) // A is wider than B
			{
				cmax.Set(bmax.x, amax.y);
			}
			else // A is thiner than B
			{
				if(amax.y > bmax.y) // A is taller than B
				{
					cmax.Set(amax.x, bmax.y);
					if(bmin.y - amin.y < amax.y - bmax.y)
					{
						norm_y.Set(0.0f, -1.0f); // A actually pen from above
					}
				}
				else // Left-Bot corner case
				{
					cmax = amax;
				}
			}
		}
		else // A penetrates B from the top
		{
			norm_y.Set(0.0f, -1.0f);
			cmin.Set(bmin.x, amin.y);
			if(amax.y < bmax.y) // A is shorter than B
			{
				if(amax.x < bmax.x) // A is shorter and thiner than B
				{
					cmax = amax;
				}
				else // A is shorter but wider than B
				{
					cmax.Set(bmax.x, amax.y);
					if(bmin.x - amin.x < amax.x - bmax.x)
					{
						norm_x.Set(-1.0f, 0.0f); // A actually pen from right
					}
				}
			}
			else
			{
				if(amax.x > bmax.x) // A is wider and taller than B
				{
					cmax = bmax;
				}
				else // Top-Left corner case
				{
					cmax.Set(amax.x, bmax.y);
				}
			}
		}
	}
	else // A penetrates B from the right
	{
		norm_x.Set(-1.0f, 0.0f);
		if(amin.y <= bmin.y) // A penetrates B from the bot
		{
			norm_y.Set(0.0f, 1.0f);
			cmin.Set(amin.x, bmin.y);
			if(amax.x < bmax.x) // A is thiner than B
			{
				if(amax.y < bmax.y) // A is thiner and shorter than B
				{
					cmax = amax;
					if(amin.x - bmin.x < bmax.x - amax.x)
					{
						norm_x.Set(1.0f, 0.0f); // A actually pen from left
					}
				}
				else // A is thiner but higher than B
				{
					cmax.Set(amax.x, bmax.y);
					if(bmin.y - amin.y < amax.y - bmax.y)
					{
						norm_y.Set(0.0f, -1.0f); // A actually pen from above
					}
				}
			}
			else
			{
				if(amax.y > bmax.y) // A is taller than B
				{
					cmax = bmax;
					if(bmin.y - amin.y < amax.y - bmax.y)
					{
						norm_y.Set(0.0f, -1.0f); // A actually pen from above
					}
				}
				else // Right-Bot corner case
				{
					cmax.Set(bmax.x, amax.y);
				}
			}
		}
		else // A penetrates B from the top
		{
			norm_y.Set(0.0f, -1.0f);
			cmin = amin;
			if(amax.x < bmax.x) // A is thiner than B
			{
				if(amax.y < bmax.y) // A is inside B
				{
					cmax = amax;
					if(amin.x - bmin.x < bmax.x - amax.x)
					{
						norm_x.Set(1.0f, 0.0f); // A actually pen from left
					}
				}
				else
				{
					cmax.Set(amax.x, bmax.y);
					if(amin.x - bmin.x < bmax.x - amax.x)
					{
						norm_x.Set(1.0f, 0.0f); // A actually pen from left
					}
				}
			}
			else // A is wider than B
			{
				if(amax.y < bmax.y) // A is shorter than B
				{
					cmax.Set(bmax.x, amax.y);
					if(amin.y - bmin.y < bmax.y - amax.y)
					{
						norm_y.Set(0.0f, 1.0f); // A actually pens be from bot
					}
				}
				else // Right-Top corner case
				{
					cmax = bmax;
				}
			}
		}
	}

	// Penetration along aixes
	const float pen_x = cmax.x - cmin.x;
	const float pen_y = cmax.y - cmin.y;

	CollisionAABR tmp;
	if(pen_x < pen_y)
	{
		tmp.info.normal = norm_x;
		tmp.info.pen = pen_x;
	}
	else
	{
		tmp.info.normal = norm_y;
		tmp.info.pen = pen_y;
	}
	// Collision point is centroid of penetration
	tmp.info.point = SciVector2(cmin.x + pen_x/2.0f, cmin.y + pen_y/2.0f);
	tmp.min = cmin;
	tmp.pen_x = pen_x;
	tmp.pen_y = pen_y;

	/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

	//out += tstr(SciGame::game->collisioner->GetCollisions()->Size());
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

	return tmp;
}
// End of Collision detection functions
// End of Aix-Aligned Bounding Rectangle