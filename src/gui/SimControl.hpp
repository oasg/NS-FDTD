#ifndef GUI_SIMCONTROL_HPP 
#define GUI_SIMCONTROL_HPP 
#include "Simulator.h"
#include <memory>
#include "GUIElement.hpp"
#include <thread>
#include "type/ImageBuffer.hpp"
#include "model/model_builder.hpp"
#include "ImageDispalyLayer.hpp"
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
            explicit SimControl(TYPE::sim_type type,std::string filePath);
            ~SimControl();
            void doSim();
            void doWatch(double imageDuration);
            void doStop();
            bool simBuild();
            inline bool ready(){ return _run; }

        private:
            std::shared_ptr<ModelBuilder> _builder;
            std::shared_ptr<Simulator> _sim;
            std::thread _cal_thread;
            std::unique_ptr<SimWatch> _watch;
            double tick;
            bool _run = false;
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
        std::string filePathName = "/home/ggrbb/code/NS-FDTD/scene/model/hairmodelDamaged.json";
        std::string filePath = "/home/ggrbb/code/NS-FDTD/scene/model/hairmodelDamaged.json";

    };


}
#endif