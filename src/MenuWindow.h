#ifndef _BUTTON_H
#define _BUTTON_H
#include"Object.h"
#include <string>
//�ｿｽ�ｿｽ�ｿｽj�ｿｽ�ｿｽ�ｿｽ[�ｿｽE�ｿｽB�ｿｽ�ｿｽ�ｿｽh�ｿｽE�ｿｽﾌ大き�ｿｽ�ｿｽ�ｿｽﾆ位置
const int MENU_WINDOW_W = WINDOW_W - MAIN_WINDOW_W;
const int MENU_WINDOW_H = MAIN_WINDOW_W;
const int MENU_WINDOW_X = 0;
const int MENU_WINDOW_Y = WINDOW_H - MAIN_WINDOW_W;

//�ｿｽ�ｿｽ�ｿｽj�ｿｽ�ｿｽ�ｿｽ[�ｿｽ�ｿｽ�ｿｽﾚの配�ｿｽu�ｿｽﾖ係
const int MENU_ITEM_EDGE = 5;
const int MENU_ITEM_NUM = 4;
const int MENU_ITEM_X   = 10;
const int MENU_ITEM_Y   = 30;
const int MENU_ITEM_W   = 180;
const int MENU_ITEM_H   = 70;
const int MENU_ITEM_D   = 30;//�ｿｽﾔ隔

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
	ofstream file;	//�ｿｽL�ｿｽ^�ｿｽp
	bool check;		//�ｿｽL�ｿｽ^�ｿｽ�ｿｽ�ｿｽ驍ｩ�ｿｽﾇゑｿｽ�ｿｽ�ｿｽ�ｿｽﾌフ�ｿｽ�ｿｽ�ｿｽO
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

//�ｿｽ{�ｿｽ^�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ鼕��ｿｽﾅ管暦ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽN�ｿｽ�ｿｽ�ｿｽX
class ButtonFactory{
private:
	static map<string, Button*> bList;
public:
	static void addButton(string tag, double val, bool check);	//�ｿｽ^�ｿｽO�ｿｽﾆ擾ｿｽ�ｿｽ�ｿｽ�ｿｽl�ｿｽ�ｿｽn�ｿｽ�ｿｽ
	static void setButton(string tag, double val);	//�ｿｽl�ｿｽﾌ変更
	static void saveButton(string tag);
	static void saveButton(string tag, double val);	//�ｿｽl�ｿｽ�ｿｽﾏ更�ｿｽ�ｿｽ�ｿｽﾄ保托ｿｽ
	static void draw(std::shared_ptr<GUI::ImageBuffer> img);	//�ｿｽ�ｿｽ�ｿｽﾗてのボ�ｿｽ^�ｿｽ�ｿｽ�ｿｽ�ｿｽ\�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	static void deleteAllButton();
};

#endif //_BUTTON_H