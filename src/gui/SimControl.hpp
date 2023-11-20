#ifndef GUI_SIMCONTROL_HPP 
#define GUI_SIMCONTROL_HPP 
#include "Simulator.h"
#include <memory>
#include "GUIElement.hpp"
namespace GUI{

    class SimControl{
        public:
            SimControl();
            ~SimControl();
            void doSim();
            void setSimulator(std::shared_ptr<Simulator> sim);
        private:
            std::shared_ptr<Simulator> _sim;
    };
    //draw SimControl menu
    class SimControlElemnet:public GUIElement{
    public:
        SimControlElemnet();
        ~SimControlElemnet();
        virtual void draw() override;
    private:
        std::shared_ptr<SimControl> _simControl;

        //
        int type = 1;
        std::vector<string> ftdt_items;

        int currentItem = 0;

    };
    //watch simulation result realtime
    class SimWatch{
    public:

    private:
    };

}
#endif