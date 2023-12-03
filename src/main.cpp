//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>


#include <string>
#include "Simulator.h"
#include "gui/Window.hpp"


void SIMThread(std::shared_ptr<Simulator> sim){
	bool run = true;
	while(run)	{
		//do simulation
		//run = sim->calc();
	}
}
int main(int argc, char *argv[]){
	//create glfw context
	GUI::GUIWindow _window(1000,600);
//	//create simulator
//	std::shared_ptr<Simulator> sim = std::make_shared<Simulator>();
//	//create threads for simulation
//	std::thread SIM_Thread(SIMThread, sim);
	_window.InitContext();
//	GUI::ImageBuffer img(sim->getFieldW(),sim->getFieldH());
    _window.doLoop();
//	SIM_Thread.join();
	return 0;
} 