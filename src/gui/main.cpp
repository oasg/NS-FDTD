//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>


#include <string>
#include "Simulator.h"
#include "gui/Window.hpp"

int main(int argc, char *argv[]){
	GUI::GUIWindow _window(1000,600);
	_window.InitContext();
    _window.doLoop();
	return 0;
} 