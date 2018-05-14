#ifndef SCIDEBUG_H
#define SCIDEBUG_H

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
inline void drawVecDebug(SciVector2 vec, SciVector2 pos, const SciPixel& col = blue_color)
{
	SciGame::game->renderer->TranslateWorld(pos);
	SciGame::game->renderer->Draw(drawLine(vec, col), 1.0f);
	SciGame::game->renderer->ResetWorld();
}
inline void drawAABRdebug(SciAABR box, const SciPixel& col = blue_color)
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
#endif /* SCI_DEBUG */

#endif /* SCIDEBUG_H */

///////////////////////////////////// DEBUGING //////////////////////////////////
//#ifdef SCI_DEBUG
//
//if(!debug_on)
//{
//	debug = CreateWindowA("STATIC", "Debug Info", WS_VISIBLE | WS_POPUP,
//		100, 0, 400, 400, NULL, NULL, NULL, NULL);
//	d_info = CreateWindowA("STATIC", "Debug Info", WS_VISIBLE | WS_CHILD,
//		0, 0, 390, 390, debug, NULL, NULL, NULL);
//	debug_on = true;
//}
//
//#endif /* SCI_DEBUG */
///////////////////////////////////// DEBUGING //////////////////////////////////
///////////////////////////////////// DEBUGING //////////////////////////////////
//#ifdef SCI_DEBUG
//
//out += tstr(1.0f / ya.mom_inert_inv);
//
//SetWindowTextA(d_info, out.c_str());
//out.clear();
//
///*if(SciGame::game->inputter->KeyIsDown('s'))
//{
//std::ofstream ofs("debug.txt");
//ofs << out;
//ofs.close();
//}*/
//
//#endif /* SCI_DEBUG */
///////////////////////////////////// DEBUGING //////////////////////////////////