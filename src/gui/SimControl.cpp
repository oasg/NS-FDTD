#include "SimControl.hpp"
#include "imgui.h"
#include <glfw/glfw3.h>
namespace GUI{
    SimControlElemnet::SimControlElemnet() {

    }
    SimControlElemnet::~SimControlElemnet() {
    }
    void SimControlElemnet::draw() {
        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("Simulator parameters");
        ImGui::Text("simulation method:");
        if (ImGui::BeginCombo("##combo", TYPE::sim_type_name[currentItem].c_str())) {
            for (int i = 0;i<TYPE::sim_type_name.size();i++) {
                const bool isSelected = (currentItem == i);
                if (ImGui::Selectable(TYPE::sim_type_name[i].c_str(), isSelected))
                    currentItem = i;
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Start")){
            _simControl = std::make_shared<SimControl>((TYPE::sim_type)currentItem);
            _simControl->doSim();
            _visualize = true;
        }
        if(_visualize){
            _simControl ->doWatch(_duration);
        }

        ImGui::End();
    }

    SimControl::SimControl(TYPE::sim_type type) {
        _sim = std::make_shared<Simulator>(type);
        _watch = std::make_unique<SimWatch>(_sim);
        tick = glfwGetTime();
    }

    SimWatch::SimWatch(std::shared_ptr<Simulator> sim) {
        int width = sim->getFieldW();
        int height = sim->getFieldH();
        //create buffer
        _sim = sim;
        _ibuffer = std::make_unique<ImageBuffer>(width,height);
        _imglayer = std::make_unique<ImageDispalyLayer>();
    }

    SimWatch::~SimWatch() {

    }

    void SimWatch::update_buffer() {
        // copy image from simulation result
        // display image
        _sim->draw(_ibuffer);
        _imglayer->update_ImageBuffer(_ibuffer);
    }

    void SimWatch::imageView() {
        _imglayer->draw();
    }

    SimControl::~SimControl() {
        //todo thread control!!!
        _cal_thread.join();
    }

    void SimControl::doSim() {
        auto cal = [=](){
            while(1){
                _sim->calc();
            }
        };
        _cal_thread = std::thread(cal);

    }

    void SimControl::doWatch(double imageDuration) {

        if (glfwGetTime() - tick > imageDuration)
        {
            _watch->update_buffer();
             tick = glfwGetTime();
        }
        _watch->imageView();
    }


}

