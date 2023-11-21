#define _USE_MATH_DEFINES
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>
#include "Simulator.h"
#include "EventState.h"
#include "Solver.h"

using namespace std;

Simulator::~Simulator() {
	cout << "Simulator Destructor" << endl;
	ButtonFactory::deleteAllButton();	//ボタンの削除, ウィンドウを閉じて終了した際に,すでに解放されたボタンを解放する恐れがある.(この順番だと大丈夫かも)
	delete solv;						//solverの削除
};
Simulator::Simulator(TYPE::sim_type type) {
    switch (type) {
        case TYPE::sim_type::FTDT_TE:
            solv = new StFDTD_TE();
            break;
        case TYPE::sim_type::FTDT_TM:
            solv = new StFDTD_TM();
            break;
        case TYPE::sim_type::NsFTDT_TE:
            solv = new NsFDTD_TE();
            break;
        case TYPE::sim_type::NsFTDT_TM:
            solv = new NsFDTD_TM();
            break;
    }
    solv->field();
}
int Simulator::calc()
{
	try {
		solv->nextTime();
		return solv->calc();
	}
	catch (char *err) {
		cout << err << endl;
		return 0;
	}
	return 1;
}

//draw gui and simulation result in the window
void Simulator::draw(std::shared_ptr<GUI::ImageBuffer> img)
{
	//	return;
	//if (((int)solv->getTime()) % 20 != 0) return;
	solv->draw(img);					//シミュレーション状況描画
	ButtonFactory::draw(img);		//ボタンを描画
}


