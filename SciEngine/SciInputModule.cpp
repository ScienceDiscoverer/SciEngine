#include "SciInputModule.h"
#include <cstring>
#include <cctype>

// Constructors
SciInputModule::SciInputModule() : mouse(0, 0)
{
	// Initiate all arrays with 0
	memset((void*)keys, 0, 256);
	memset((void*)mbutts, 0, 3);
	Update();
}
// End of Constructors

// Key update functions
void SciInputModule::PressKey(uchar key)
{
	if(isupper(key))
	{
		key = (uchar)tolower(key);
	}
	
	keys[key] = true;
	pressed_k[key] = true;
}

void SciInputModule::ReleaseKey(uchar key)
{
	if(isupper(key))
	{
		key = (uchar)tolower(key);
	}
	
	keys[key] = false;
	released_k[key] = true;
}
// End of Key update functions

// Mouse update functions
void SciInputModule::PressMbutt(MouseButton b)
{
	mbutts[b] = true;
	pressed_mb[b] = true;
}

void SciInputModule::ReleaseMbutt(MouseButton b)
{
	mbutts[b] = false;
	released_mb[b] = true;
}
// End of Mouse update functions

// Input state update
void SciInputModule::Update()
{
	memset((void*)pressed_k, 0, 256);
	memset((void*)released_k, 0, 256);
	memset((void*)pressed_mb, 0, 3);
	memset((void*)released_mb, 0, 3);
}
// End of Input state update