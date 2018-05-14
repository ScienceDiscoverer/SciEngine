//#include "BallsGame.h"
//#include "../ActiveGame.h"
///*////////////////// DEBUG INCLUDES ///////////////////
////#include <windows.h>
//#include <string>
////MessageBox(NULL, "back not loded", "Error", MB_OK | MB_ICONERROR);
//HWND debug_m, d_info_m;
//bool debug_on_m = false;
//typedef std::string str;
//template <class T>
//str tstr(T srs) { return std::to_string(srs); }
///////////////////// DEBUG INCLUDES ///////////////////*/
///*/////////////////////////////////// DEBUGING //////////////////////////////////
//if(!debug_on_m)
//{
//debug_m = CreateWindow("STATIC", "Debug Info", WS_VISIBLE | WS_POPUP,
//100, 0, 400, 400, NULL, NULL, NULL, NULL);
//d_info_m = CreateWindow("STATIC", "Debug Info", WS_VISIBLE | WS_CHILD,
//0, 0, 390, 390, debug_m, NULL, NULL, NULL);
//debug_on_m = true;
//}
//
//str size;
//
//SetWindowText(d_info_m, size.c_str());
//
///////////////////////////////////// DEBUGING //////////////////////////////////*/
//
//// Clever trick: implement base class' static function
//// to let it hold the static pointer to this game
//#ifdef BALLS
//bool SciGame::Create()
//{
//	SciGame::game = new BallsGame;
//	return (SciGame::game != nullptr);
//}
//#endif
//
//// Constructors
///* Need a way to dinimically increse object array!
//To achive this, need to make collision object
//That will hold pointer to array of objects
//And index of object and will use array [] operator */
//BallsGame::BallsGame() : balls(100), w_left(0), w_right(1)
//{
//	// Load White Ball...
//	//ball.LoadFile("img/sball.tga", 20, 20);
//	ball.LoadFile("img/box.tga", 30, 30);
//	//ball.LoadFile("img/onna.tga", 14, 16);
//	//ball.LoadFile("img/arrow_prot.tga", 30, 8);
//}
//// End of Constructors
//
//// Virtual interface functions
//bool BallsGame::Initialize(ScreenRes scr)
//{
//	renderer = new SciRenderer2D(scr);
//	inputter = new SciInputModule;
//	collisioner = new SciCollision2D;
//	physicist = new SciPhysics2D;
//
//	// Connect Physics Engine with Collision System
//	physicist->ConnectCollisions(collisioner->GetCollisions());
//	// Optionally, add global forces to Physics Engine here
//
//	collisioner->AddObject(&earth);
//	collisioner->AddObject(&w_left);
//	collisioner->AddObject(&w_right);
//	physicist->AddObject(&earth);
//	physicist->AddObject(&w_left);
//	physicist->AddObject(&w_right);
//
//	return true;
//}
//
//void BallsGame::Update(float dt)
//{
//	// Spawn Ball
//	if(inputter->KeyWasReleased('b'))
//	{
//		Ball ball(&ball, renderer->CurScrSize());
//		balls.Push(ball);
//		collisioner->AddObject(&balls.PeekBack());
//		physicist->AddObject(&balls.PeekBack());
//	}
//
//	// Enable/Disable gravity
//	//const SciVector2 f(0.0f, -36320.92f); // IRL meter
//	const SciVector2 f(0.0f, -196.133f); // 20 px = meter
//	if(inputter->KeyWasPressed('g'))
//	{
//		physicist->AddGlobalForce(Custom, f);
//	}
//	if(inputter->KeyWasPressed('f'))
//	{
//		physicist->RemoveGlobalForce(Custom, f);
//	}
//	inputter->Update();
//
//	SciVector2 c_force(zero_vec2); // Control Force
//	if(inputter->KeyIsDown('i'))
//	{
//		c_force += SciVector2(0.0f, 500.0f);
//	}
//	if(inputter->KeyIsDown('k'))
//	{
//		c_force += SciVector2(0.0f, -500.0f);
//	}
//	if(inputter->KeyIsDown('j'))
//	{
//		c_force += SciVector2(-500.0f, 0.0f);
//	}
//	if(inputter->KeyIsDown('l'))
//	{
//		c_force += SciVector2(500.0f, 0.0f);
//	}
//	balls[0].ApplyForce(c_force);
//
//	float c_torq = 0.0f; // Control Torque
//	if(inputter->KeyIsDown('u'))
//	{
//		c_torq = 500.0f;
//	}
//	if(inputter->KeyIsDown('o'))
//	{
//		c_torq = -500.0f;
//	}
//	balls[0].ApplyTorque(c_torq);
//	
//
//	collisioner->DetectCollisions();
//
//	physicist->Integrate(dt);
//	
//	// Update all balls
//	for(int i = 0; i < balls.Size(); ++i)
//	{
//		balls[i].Update();
//	}
//}
//
//void BallsGame::Render(float alpha)
//{
//	renderer->ClearBuffer();
//
//	earth.Render(alpha);
//	w_left.Render(alpha);
//	w_right.Render(alpha);
//
//	for(int i = 0; i < balls.Size(); ++i)
//	{
//		balls[i].Render(alpha);
//	}
//
//	renderer->DrawBuffer();
//}
//// End of Virtual interface functions