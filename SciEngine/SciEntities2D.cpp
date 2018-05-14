#include <cassert>
#include "SciEntities2D.h"
#include "SciMath/SciMath.h"

// SciEntity
// Core operations
// Compose Model to World transformation matrix
SciMatrix3x3 SciEntity::ModWorld() const
{
	SciMatrix3x3 tmp;
	tmp.SetRotation(orient);
	tmp.SetTranslation(pos);
	return tmp;
}

// Initialize/resize rotated sprite to correct dimentions
void SciEntity::SetRotSprite(const SciSprite& os, const SciVector2& ct, const SciMatrix2x2 or)
{
	// Find original sprite deformation due to center shift
	const float w = (float)os.Width() * 2.0f + sciAbs(ct.x);
	const float h = (float)os.Height() * 2.0f + sciAbs(ct.y);
	// Calculate diameter of deformed original sprite
	const float sum_sqr = w*w + h*h;
	const int diam = sciFloorI(sciSqrt(sum_sqr));
	int diam_w = diam, diam_h = diam;
	// Ensure that diameter has same even/oddness as width/height
	if((diam % 2 == 0 && os.Width() % 2 != 0) || (diam % 2 != 0 && os.Width() % 2 == 0))
	{
		++diam_w;
	}
	if((diam % 2 == 0 && os.Height() % 2 != 0) || (diam % 2 != 0 && os.Height() % 2 == 0))
	{
		++diam_h;
	}
	// Initialize/Resize rotation sprite
	if(!rot_sprite.Init(diam_w, diam_h))
	{
		rot_sprite.ReSize(diam_w, diam_h);
	}
	rotateImage(os, rot_sprite, or, ct);
}

void SciEntity::SetRotSprite(const SciSprite& os)
{
	// Calculate diameter of original sprite
	const int sum_sqr = os.Width()*os.Width() + os.Height()*os.Height();
	const int diam = sciFloorI(sciSqrt((float)sum_sqr));
	int diam_w = diam, diam_h = diam;
	// Ensure that diameter has same even/oddness as width/height
	if((diam % 2 == 0 && os.Width() % 2 != 0) || (diam % 2 != 0 && os.Width() % 2 == 0))
	{
		++diam_w;
	}
	if((diam % 2 == 0 && os.Height() % 2 != 0) || (diam % 2 != 0 && os.Height() % 2 == 0))
	{
		++diam_h;
	}
	// Initialize/Resize rotation sprite
	if(!rot_sprite.Init(diam_w, diam_h))
	{
		rot_sprite.ReSize(diam_w, diam_h);
	}
	rotateImage(os, rot_sprite, orient);
}

void SciEntity::SetRotSprite()
{
	SetRotSprite(sprite);
}

void SciEntity::RotateSprite(const SciSprite& orig, const SciMatrix2x2& or, const SciVector2& ct)
{
	if(or != past_orient)
	{
		rotateImage(orig, rot_sprite, or, ct);
		past_orient = or;
	}
}

void SciEntity::RotateSprite(const SciSprite& orig, const SciMatrix2x2& or)
{
	if(or != past_orient)
	{
		rotateImage(orig, rot_sprite, or);
		past_orient = or;
	}
}

void SciEntity::RotateSprite(const SciSprite& orig)
{
	RotateSprite(orig, orient);
}

void SciEntity::RotateSprite()
{
	RotateSprite(sprite, orient);
}
// End of Core operations
// End of SciEntity

// SciCollisionEntity
// Collision update functions
void SciCollisionEntity::UpdateBound(const SciVector2& trans)
{
	b_rec.Update(trans);
}
// End of SciCollisionEntity
// End of Collision update functions

// SciPhysicsEntity
// Core operations
void SciPhysicsEntity::CalcMomInertInv(const SciSprite& spr)
{
	// Find sprite's center of mass
	const SciVector2 c_mass = spr.Center();
	mom_inert_inv = 0.0f;
	int solid_pixs = 0;

	// Find out number of solid pixels in the sprite
	for(int i = 0; i < spr.Size(); ++i)
	{
		if(spr[i].a)
		{
			++solid_pixs;
		}
	}

	assert(solid_pixs);
	const float pix_m = mass / solid_pixs;

	// Sum up all (mass * squared distance from c. mass) of pixels
	for(int i = 0; i < spr.Height(); ++i)
	{
		for(int j = 0; j < spr.Width(); ++j)
		{
			if(spr(j, i).a) // Summ only non-fully transperent pixels
			{
				const SciVector2 pix_pos((float)j, (float)i);
				const SciVector2 r = pix_pos - c_mass;
				mom_inert_inv += pix_m * r.MagSqr();
			}
		}
	}

	mom_inert_inv = 1.0f / mom_inert_inv;
}
// End of SciPhysicsEntity
// End of Core operations