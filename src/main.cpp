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
// �ϐ��̐錾
//----------------------------------------------------
const int WindowPositionX = 200;  //��������E�B���h�E�ʒu��X���W
const int WindowPositionY = 200;  //��������E�B���h�E�ʒu��Y���W
const char WindowTitle[] = "�V�~�����[�V����";  //�E�B���h�E�̃^�C�g��


//----------------------------------------------------
// �֐��v���g�^�C�v�i��ɌĂяo���֐����ƈ����̐錾�j
//----------------------------------------------------
void Initialize(void);   //�����ݒ莞�ɌĂяo���֐�
void Idle(void);         //�A�C�h�����ɌĂяo���֐�
void Display(void);      //��ʕ`�掞�ɌĂяo���֐�
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
// �����ݒ�̊֐�
//----------------------------------------------------
void Initialize(void){
	
}
//----------------------------------------------------
// �A�C�h�����ɌĂяo�����֐�
//----------------------------------------------------
void Idle(){
	if( getKeyState() == _KEY_ESC){
		//delete wave_simulator;
		exit(0);
	}
	
// 	Sleep(1);
//   setKeyState(-1);			//�L�[�{�[�h������
//   setSpecialKeyState(-1);
//   glutPostRedisplay(); //glutDisplayFunc()���P����s����
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
// �`��̊֐�
//----------------------------------------------------
void Display(void) {
	// glClear(GL_COLOR_BUFFER_BIT);

	// wave_simulator->draw();

	// glutSwapBuffers(); //glutInitDisplayMode(GLUT_DOUBLE)�Ń_�u���o�b�t�@�����O�𗘗p��
	// glFlush();
}
