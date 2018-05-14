#include <SciMath.h>
#include "ArcherGame.h"
#include "../ActiveGame.h"
////////////////// DEBUG INCLUDES ///////////////////
//#define SCI_DEBUG
#ifdef SCI_DEBUG
#include <windows.h>
#include <string>
#include <fstream>
#include "SciGame.h"
typedef std::string str;

HWND debug, d_info;
bool debug_on = false;
int frames = 0;
str out;


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

float tot_ang;
SciVector2 dirg, w_posg, mouseg;
#endif /* SCI_DEBUG */
/////////////////// DEBUG INCLUDES ///////////////////
/////////////////////////////////// DEBUGING //////////////////////////////////
#ifdef SCI_DEBUG

w_posg = w_pos;
mouseg = mouse;
dirg = w_orient.GetColumn(1);

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

// Clever trick: implement base class' static function
// to let it hold the static pointer to this game
#ifdef ARCHER
bool SciGame::Create()
{
	SciGame::game = new ArcherGame;
	return (SciGame::game != nullptr);
}
#endif

//Constructors
ArcherGame::ArcherGame() : w_arrows(100)
{
	yumiko.LinkWorldArrows(&w_arrows);
}
//End of Constructors

//Virtual interface functions
bool ArcherGame::Initialize(ScreenRes scr)
{
	renderer = new SciRenderer2D(scr);
	inputter = new SciInputModule;
	collisioner = new SciCollision2D;
	physicist = new SciPhysics2D(16.875f);

	physicist->ConnectCollisions(collisioner->GetCollisions());
	physicist->AddGlobalForce(Gravity);
	//physicist->ConfigAir(false, 0.1f);

	collisioner->AddObject(&earth);
	collisioner->AddObject(&yumiko);
	physicist->AddObject(&earth);
	physicist->AddObject(&yumiko);

	// Load dummy background...
	/*SciResourceNan<SciPixel> backgrnd;
	backgrnd.LoadFile("img/back.tga", 350, 350);
	renderer->SetBackground(backgrnd);*/
	return true;
}

void ArcherGame::Update(float dt)
{
	collisioner->DetectCollisions();
	physicist->Integrate(dt);
	
	yumiko.Update(dt);
	for(int i = 0; i < w_arrows.Size(); ++i)
	{
		w_arrows[i].Update();
	}
}

void ArcherGame::Render(float alpha)
{
	renderer->ClearBuffer();
	
	earth.Render(alpha);
	yumiko.Render(alpha);

	for(int i = 0; i < w_arrows.Size(); ++i)
	{
		w_arrows[i].Render(alpha);
	}

	renderer->DrawBuffer();
}
//End of Virtual interface functions