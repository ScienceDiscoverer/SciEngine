#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "opengl32.lib")

#include <windows.h>
#include <gl/GL.h>
#include "SciRenderer2D.h"
#include "SciMath/SciMath.h"
/*////////////////// DEBUG INCLUDES ///////////////////
//#include <windows.h>
#include <string>
//MessageBox(NULL, "back not loded", "Error", MB_OK | MB_ICONERROR);
HWND debug_m, d_info_m;
bool debug_on_m = false;
typedef std::string str;
template <class T>
str tstr(T srs) { return std::to_string(srs); }
/////////////////// DEBUG INCLUDES ///////////////////*/
/*/////////////////////////////////// DEBUGING //////////////////////////////////
str size(str("Rotated sprite size: ") + tstr(rot_sprite.Size()));
size += str("\nWidth: ") + tstr(rot_sprite.Width()) + str(" Height: ") + tstr(rot_sprite.Height());

if(!debug_on_m)
{
debug_m = CreateWindowA("STATIC", "Debug Info", WS_VISIBLE | WS_POPUP,
100, 0, 400, 400, NULL, NULL, NULL, NULL);
d_info_m = CreateWindowA("STATIC", size.c_str(), WS_VISIBLE | WS_CHILD,
0, 0, 390, 390, debug_m, NULL, NULL, NULL);
debug_on_m = true;
}

SetWindowTextA(d_info_m, size.c_str());

/////////////////////////////////// DEBUGING //////////////////////////////////*/

// Constructors
SciRenderer2D::SciRenderer2D(ScreenRes scr)
{
	Initialize(scr);
}
// End of Constructors

// Interface functions
// Basic initialization
bool SciRenderer2D::Initialize(ScreenRes scr)
{
	if(scr_buffer.Size())
	{
		return false; // Renderer is already initialized
	}
	
	bool scr_s = scr_buffer.Init(scr.w, scr.h);
	bool buf_s = b_grnd.Init(scr.w, scr.h);
	b_grnd.Fill(def_bg_color);
	ClearBuffer();
	buf_res = scr;
	return scr_s & buf_s;
}

bool SciRenderer2D::Resize(ScreenRes scr)
{
	// Handle initial WinAPI resize (no actual resize)
	if(scr.w == scr_buffer.Width() && scr.h == scr_buffer.Height())
	{
		return true;
	}

	bool scr_s = scr_buffer.ReSize(scr.w, scr.h);
	bool buf_s = b_grnd.ReSize(scr.w, scr.h);
	b_grnd.Fill(def_bg_color);
	ClearBuffer();
	buf_res = scr;
	return scr_s & buf_s;
}

// Background dimentions must be equal to screen buffer dimentions 
bool SciRenderer2D::SetBackground(const SciSprite& new_b_grnd)
{
	if(b_grnd.Size() == new_b_grnd.Size())
	{
		b_grnd = new_b_grnd;
		return true;
	}
	else
	{
		return false;
	}
}

// Drawing functions
void SciRenderer2D::Draw(const SciSprite& sprite, float alpha)
{
	// Center [x,y] of sprite is in bottom-left corner of central pixel
	const int cx = sprite.Width()/2;
	const int cy = sprite.Height()/2;

	// Sprite's origin - translate -0.5 to accommodate center's offset
	const int x = sciFloorI(WorldTranslate.x - 0.5f);
	const int y = sciFloorI(WorldTranslate.y - 0.5f);

	for(int i = 0; i < sprite.Height(); ++i)
	{
		const int pix_y = y + i - cy;

		for(int j = 0; j < sprite.Width(); ++j)
		{
			const int pix_x = x + j - cx;

			if(PixelInsideFrust(pix_x, pix_y))
			{
				const SciPixel& spr_p = sprite(j, i);
				if(!spr_p.a)
				{
					continue; // Ignore pixel, it's 100% invisible
				}
				SciPixel& buf_p = scr_buffer(pix_x, pix_y);
				if(spr_p.a == 1.0f)
				{
					buf_p = spr_p; // Pixel is solid, copy it directly
					continue;
				}

				buf_p.a = spr_p.GetInverseAlpha();
				buf_p = BlendAlpha(buf_p, spr_p);
			}
		}
	}
}

// Buffer manipulations
// This function must not be called on uninitialized buffer!
void SciRenderer2D::ClearBuffer()
{
	// Use memcpy for very fast buffer clearing
	memcpy((void*)scr_buffer.GetData(), (void*)b_grnd.GetData(), scr_buffer.Size()*sizeof(SciPixel));
}

// Send Buffer to Graphics API
void SciRenderer2D::DrawBuffer()
{
	glDrawPixels(scr_buffer.Width(), scr_buffer.Height(), GL_RGBA, GL_FLOAT, scr_buffer.GetData());
}
// End of Interface functions

// Private functions
// Test if pixel is inside view frustum (camera, buffer, etc.)
bool SciRenderer2D::PixelInsideFrust(int x, int y) const
{
	// View frustum constrains !!! MAKE THIS AUTOMATIC PRIVATE FUNC FROM CAMERA TRANSLATION !!!
	const int fx0 = 0, fy0 = 0; // bottom-left corner                                       !!!
	const int fx1 = buf_res.w;  //                                                 !!!
	const int fy1 = buf_res.h; // top-right corner                                !!!
	//.........................................................................................

	return x >= fx0 && y >= fy0 && x < fx1 && y < fy1;
}

// Perform alpha blending, col0 is a background
SciPixel SciRenderer2D::BlendAlpha(const SciPixel& col0, const SciPixel& col1) const
{
	return col0.ScaleColor(col0.a) + col1.ScaleColor(col1.a);
}
// End of Private functions