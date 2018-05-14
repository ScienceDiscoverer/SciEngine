#ifndef SCIRENDERMODULE_H
#define SCIRENDERMODULE_H

// Abstract interface class
class SciRenderModule
{
	// Basic Initialization
	virtual bool Initialize(int scr_w, int scr_h) = 0;
	virtual bool Resize(int scr_w, int scr_h) = 0;

	// Draw functions overloads for different renderers
	virtual bool Draw()
};

#endif /* SCIRENDERMODULE_H */
