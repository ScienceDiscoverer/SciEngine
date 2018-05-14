#ifndef SCIINPUTMODULE_H
#define SCIINPUTMODULE_H

#include "SciMath/SciVector2.h"

// Typedefs
typedef unsigned char uchar;

// Enums
enum MouseButton { Left, Right, Middle };

// Helper structs
struct MousePos
{
	// Constructors
	MousePos() = default;
	MousePos(int xp, int yp) : x(xp), y(yp) {}

	// Custom convertions
	operator SciVector2() const { return SciVector2((float)x, (float)y); }

	// Data
	int x, y;
};

class SciInputModule
{
public:
	// Constructors
	SciInputModule();

	// Key status functions
	bool KeyIsDown(uchar key) const { return keys[key]; }
	bool KeyIsUp(uchar key) const { return !keys[key]; }

	// Key past status after update, before next update
	bool KeyWasPressed(uchar key) const { return pressed_k[key]; }
	bool KeyWasReleased(uchar key) const { return released_k[key]; }

	// Key update functions
	void PressKey(uchar key);
	void ReleaseKey(uchar key);

	// Mouse status functions
	bool MouseDown(MouseButton b) const { return mbutts[b]; }
	bool MouseUp(MouseButton b) const { return !mbutts[b]; }
	const MousePos& GetMousePos() const { return mouse; }

	// Mouse past status after update, before next update
	bool MouseWasPressed(MouseButton b) const { return pressed_mb[b]; }
	bool MouseReleased(MouseButton b) const { return released_mb[b]; }

	// Mouse update functions
	void PressMbutt(MouseButton b);
	void ReleaseMbutt(MouseButton b);
	void UpdateMouse(const MousePos& pos) { mouse = pos; }

	// Input state update
	void Update();

private:
	// Data
	bool keys[256];
	bool pressed_k[256];
	bool released_k[256];
	MousePos mouse;
	bool mbutts[3];
	bool pressed_mb[3];
	bool released_mb[3];
};

#endif /* SCIINPUTMODULE_H */