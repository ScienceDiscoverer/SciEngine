#ifndef SCIRENDERMODULE_H
#define SCIRENDERMODULE_H

#include "Utilities/SciResourceNan.h"
#include "SciMath/SciMatrix3x3.h"

// Helper structs
struct ScreenRes
{
	// Constructors
	ScreenRes() : w(0), h(0) {}
	ScreenRes(int width, int height) : w(width), h(height) {}

	// Data
	int w, h;
};

class SciRenderModule
{
public:
	// Virtual destructor for polymorthic class
	virtual ~SciRenderModule() = default;

	// Basic Initialization
	virtual bool Initialize(ScreenRes scr) = 0;
	virtual bool Resize(ScreenRes scr) = 0;
	virtual bool SetBackground(const SciResourceNan<SciPixel>& new_b_grnd) = 0;

	// Draw functions overloads for different renderers
	// 2D renderer, cam and world transforms are separate calls
	virtual void Draw(const SciResourceNan<SciPixel>& sprite, float alpha) = 0;

	// Transform interface
	// 2D renderer
	virtual void TranslateWorld(const SciVector2& model_world) = 0;
	virtual void ResetWorld() = 0;
	virtual void TransformView(const SciMatrix3x3& world_view) = 0;
	virtual void ResetView() = 0;

	// Buffer manipulations
	virtual void ClearBuffer() = 0;
	virtual void DrawBuffer() = 0;

	// Usefull information
	virtual const ScreenRes* CurScrSize() = 0;
};

#endif /* SCIRENDERMODULE_H */
