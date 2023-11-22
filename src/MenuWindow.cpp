#include "EventState.h"
#include"MenuWindow.h"

Point::Point(int _x,int _y)
	:x(_x),y(_y)
{
}

int Point::getX(){
	return x;
}

int Point::getY(){
	return y;
}

void Point::setX(int _x){
	x = _x;
}

void Point::setY(int _y){
	y = _y;
}

/* �ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ`�ｿｽ�ｿｽ*/
void drawBitmapString(void *font, string s, int x, int y)
{
	double _x1 =  2.0*x/WINDOW_W - 1;
	double _y1 = -2.0*y/WINDOW_H + 1;
	
// 	glPushAttrib(GL_CURRENT_BIT);
// 	glRasterPos2d(_x1, _y1);		/* �ｿｽ`�ｿｽ�ｿｽﾊ置 */
//   /* �ｿｽr�ｿｽb�ｿｽg�ｿｽ}�ｿｽb�ｿｽv�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽﾌ描�ｿｽ�ｿｽ */
// 	for(unsigned int i=0; i<s.size() ;i++)
// 		glutBitmapCharacter(font, s[i]);

//   glPopAttrib();

}

map<string, Button*> ButtonFactory::bList;

Button::Button(int _x,int _y, int _w, int _h, string _tag, double _val, bool _check)
:Point(_x,_y), width(_w), height(_h), tag(_tag), value(_val), check(_check)
{
	if(check)
		file.open(tag+".txt");
	cout << "Button Constructor" << endl;
}

double Button::getState(){
	return value;
}

void Button::setState(double s){
	value = s;
}

int Button::calc(){
	char _key = getKeyState();

	return 1; 
}

void Button::draw(std::shared_ptr<GUI::ImageBuffer> img){
	//TODO DRAW 
	// glColor3d(1, 1, 1);  //�ｿｽF�ｿｽﾍ費ｿｽ�ｿｽﾅ描�ｿｽ�ｿｽ TODO�ｿｽ@�ｿｽw�ｿｽi�ｿｽﾉゑｿｽ�ｿｽ�ｿｽﾄは隠�ｿｽ�ｿｽ�ｿｽ
	// drawBitmapString(GLUT_BITMAP_HELVETICA_18, tag, x,y);
	// drawBitmapString(GLUT_BITMAP_HELVETICA_18, to_s(value), x + 10*tag.size() ,y);
	//if(check)	file << value << endl;	//�ｿｽL�ｿｽ^�ｿｽ�ｿｽ�ｿｽ�ｿｽ鼾��ｿｽﾍフ�ｿｽ@�ｿｽC�ｿｽ�ｿｽ�ｿｽﾉ擾ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
}

void Button::save(){
	if(file)
		file << value << endl;	//�ｿｽL�ｿｽ^�ｿｽ�ｿｽ�ｿｽ�ｿｽ鼾��ｿｽﾍフ�ｿｽ@�ｿｽC�ｿｽ�ｿｽ�ｿｽﾉ擾ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ�ｿｽ
	else
		cout << "no file error" << endl;
}

void ButtonFactory::addButton(string _tag, double _val, bool _check){
	if(bList.count(_tag) == 0)	//�ｿｽ�ｿｽ�ｿｽﾝゑｿｽ�ｿｽﾈゑｿｽ�ｿｽ鼾�
		bList[_tag] = new Button(MENU_ITEM_X + MAIN_WINDOW_X, MENU_ITEM_D*bList.size() + MENU_ITEM_Y + MAIN_WINDOW_Y, MENU_ITEM_W, MENU_ITEM_H,  _tag, _val, _check);
	else
		bList[_tag]->setState(_val);
}

void ButtonFactory::setButton(string _tag, double _val){
	addButton(_tag, _val, false);
}

void ButtonFactory::draw(std::shared_ptr<GUI::ImageBuffer> img){
	map<string, Button*>::iterator it = bList.begin();
	while( it != bList.end() )
	{
		(*it).second->draw(img);
		++it;
	}
}

void ButtonFactory::saveButton(string _tag){
	bList[_tag]->save();
}

void ButtonFactory::saveButton(string _tag, double _val){
	addButton(_tag, _val, true);
}

void ButtonFactory::deleteAllButton(){
	map<string, Button*>::iterator it = bList.begin();

	while (it != bList.end()) {
		std::map<string, Button*>::iterator toErase = it;
		++it;
		delete (*toErase).second;
	}
	bList.clear();
}
