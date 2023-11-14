//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>


#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string>
#include "Simulator.h"
#include "EventState.h"
#include "gui/Window.hpp"
#include "ImageBuffer.hpp"
#include <thread>


//----------------------------------------------------
// 変数の宣言
//----------------------------------------------------
const int WindowPositionX = 200;  //生成するウィンドウ位置のX座標
const int WindowPositionY = 200;  //生成するウィンドウ位置のY座標
const char WindowTitle[] = "シミュレーション";  //ウィンドウのタイトル


//----------------------------------------------------
// 関数プロトタイプ（後に呼び出す関数名と引数の宣言）
//----------------------------------------------------
void Initialize(void);   //初期設定時に呼び出す関数
void Idle(void);         //アイドル時に呼び出す関数
void Display(void);      //画面描画時に呼び出す関数
void Mouse(int _button , int _state , int _x , int _y);
void Motion(int _x,int _y);
void KeyBoard(unsigned char key, int x, int y);
void SpecialKeyBoard(int key, int x, int y);

void SIMThread(std::shared_ptr<Simulator> sim){
	bool run = true;
	while(run)	{
		//do simulation
		run = sim->calc();
	}
}
int main(int argc, char *argv[]){
	//create glfw context
	GUI::GUIWindow _window(1000,600);
	//create simulator
	std::shared_ptr<Simulator> sim = std::make_shared<Simulator>();
	//create threads for simulation
	std::thread SIM_Thread(SIMThread, sim);
	_window.InitContext();
	GUI::ImageBuffer img(400,400);
	_window.doLoop(img);
	SIM_Thread.join();
	return 0;
} 
//----------------------------------------------------
// 初期設定の関数
//----------------------------------------------------
void Initialize(void){
	
}
//----------------------------------------------------
// アイドル時に呼び出される関数
//----------------------------------------------------
void Idle(){
	if( getKeyState() == _KEY_ESC){
		//delete wave_simulator;
		exit(0);
	}
	
// 	Sleep(1);
//   setKeyState(-1);			//キーボード初期化
//   setSpecialKeyState(-1);
//   glutPostRedisplay(); //glutDisplayFunc()を１回実行する
}

void Mouse(int _button , int _state , int _x , int _y){
	setMouseState( _button,  _state, _x, _y);
}

void Motion(int _x,int _y){
	setPos(_x,_y);
}

void KeyBoard(unsigned char key, int x, int y){
	setKeyState(key);
}

void SpecialKeyBoard(int key, int x, int y){
	setSpecialKeyState(key);
}
//----------------------------------------------------
// 描画の関数
//----------------------------------------------------
void Display(void) {
	// glClear(GL_COLOR_BUFFER_BIT);

	// wave_simulator->draw();

	// glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)でダブルバッファリングを利用可
	// glFlush();
}
