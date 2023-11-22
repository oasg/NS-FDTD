#ifndef GUI_SIMCONTROL_HPP 
#define GUI_SIMCONTROL_HPP 
#include "Simulator.h"
#include <memory>
#include "GUIElement.hpp"
#include <thread>
#include "ImageBuffer.hpp"
namespace GUI{

    //watch simulation result realtime
    class SimWatch{
        public:
            explicit SimWatch(std::shared_ptr<Simulator> sim);
            void update_buffer();
            void imageView();
            ~SimWatch();
        private:
            std::shared_ptr<Simulator> _sim;
            std::shared_ptr<ImageBuffer> _ibuffer;
            std::unique_ptr<ImageDispalyLayer> _imglayer;
    };

    class SimControl{
        public:
            explicit SimControl(TYPE::sim_type type);
            ~SimControl();
            void doSim();
            void doWatch(double imageDuration);

        private:
            std::shared_ptr<Simulator> _sim;
            std::thread _cal_thread;
            std::unique_ptr<SimWatch> _watch;
            double tick;
    };

    //draw SimControl menu
    class SimControlElemnet:public GUIElement{
    public:
        SimControlElemnet();
        ~SimControlElemnet();
        virtual void draw() override;
    private:
        //create sim control
        std::shared_ptr<SimControl> _simControl;
        int currentItem = 0;
        float _duration = 0.1;
        bool _visualize = false;


    };


}
#endif