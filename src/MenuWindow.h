#ifndef _BUTTON_H
#define _BUTTON_H
#include"Object.h"
#include <string>
//���j���[�E�B���h�E�̑傫���ƈʒu
const int MENU_WINDOW_W = WINDOW_W - MAIN_WINDOW_W;
const int MENU_WINDOW_H = MAIN_WINDOW_W;
const int MENU_WINDOW_X = 0;
const int MENU_WINDOW_Y = WINDOW_H - MAIN_WINDOW_W;

//���j���[���ڂ̔z�u�֌W
const int MENU_ITEM_EDGE = 5;
const int MENU_ITEM_NUM = 4;
const int MENU_ITEM_X   = 10;
const int MENU_ITEM_Y   = 30;
const int MENU_ITEM_W   = 180;
const int MENU_ITEM_H   = 70;
const int MENU_ITEM_D   = 30;//�Ԋu

class Point: public Object{
protected:
	int x,y;
public:
	Point(int _x,int _y);
	~Point(){	cout<< "Point Destructor" << endl;	};
	int getX();
	int getY();
	void setX(int _x);
	void setY(int _y);
};

class Button :public Point{
private:
	ofstream file;	//�L�^�p
	bool check;		//�L�^���邩�ǂ����̃t���O
protected:
	int width,height;
	string tag;
	double value;
public:
	Button(int _x, int _y, int _w, int _h, string _tag, double _val, bool _chexk);
	~Button(){	cout << "Button Destructor" << endl;	};
	double getState();
	void setState(double _state);
	void save();
	int calc();
	void draw(std::shared_ptr<GUI::ImageBuffer> img);
};

void drawBitmapString(void *font, char *string, int x, int y);

//�{�^�����ꊇ�ŊǗ�����N���X
class ButtonFactory{
private:
	static map<string, Button*> bList;
public:
	static void addButton(string tag, double val, bool check);	//�^�O�Ə����l��n��
	static void setButton(string tag, double val);	//�l�̕ύX
	static void saveButton(string tag);
	static void saveButton(string tag, double val);	//�l��ύX���ĕۑ�
	static void draw(std::shared_ptr<GUI::ImageBuffer> img);	//���ׂẴ{�^����\��������
	static void deleteAllButton();
};

#endif //_BUTTON_H