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
	ButtonFactory::deleteAllButton();	//繝懊ち繝ｳ縺ｮ蜑企勁, 繧ｦ繧｣繝ｳ繝峨え繧帝哩縺倥※邨ゆｺ�縺励◆髫帙↓,縺吶〒縺ｫ隗｣謾ｾ縺輔ｌ縺溘�懊ち繝ｳ繧定ｧ｣謾ｾ縺吶ｋ諱舌ｌ縺後≠繧�.(縺薙�ｮ鬆�逡ｪ縺�縺ｨ螟ｧ荳亥､ｫ縺九ｂ)
	delete solv;						//solver縺ｮ蜑企勁
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
	solv->draw(img);			
	ButtonFactory::draw(img);	
}


