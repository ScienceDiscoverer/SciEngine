#ifndef SCIRENDERER2D_H
#define SCIRENDERER2D_H

#include "SciRenderModule.h"

// Constants
// Default background color
const SciPixel def_bg_color(0.167968f, 0.515625f, 0.816406f, 1.0f);

class SciRenderer2D : public SciRenderModule
{
public:
	// Constructors
	SciRenderer2D() = default;
	SciRenderer2D(ScreenRes scr);
	
	// Interface functions
	// Basic initialization
	virtual bool Initialize(ScreenRes scr);
	virtual bool Resize(ScreenRes scr);
	virtual bool SetBackground(const SciSprite& new_b_grnd); // Sizes of bgrnd and screen must be equal
	// Drawing functions
	virtual void Draw(const SciSprite& sprite, float alpha);
	// Transform interface
	virtual void TranslateWorld(const SciVector2& model_world) { WorldTranslate = model_world; }
	virtual void ResetWorld() { WorldTranslate = zero_vec2; }
	virtual void TransformView(const SciMatrix3x3& world_view) { ViewTransform = world_view; }
	virtual void ResetView() { ViewTransform.Identity(); }
	// Buffer manipulations
	virtual void ClearBuffer(); // This function must not be called on uninitialized buffer!
	virtual void DrawBuffer(); // Send Buffer to Graphics API
	// Usefull information
	virtual const ScreenRes* CurScrSize() { return &buf_res; }

private:
	// Private functions
	bool PixelInsideFrust(int x, int y) const; // Test if pixel is inside view frustum (camera, buffer, etc.)
	SciPixel BlendAlpha(const SciPixel& col0, const SciPixel& col1) const;

	// Data
	SciImage scr_buffer;
	SciImage b_grnd; // Background
	SciVector2 WorldTranslate;
	SciMatrix3x3 ViewTransform;
	ScreenRes buf_res; // Screen Buffer resolution
};

#endif /* SCIRENDERER2D_H */