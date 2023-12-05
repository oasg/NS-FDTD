#include "SimControl.hpp"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include "ImGuiFileDialog.h"

namespace GUI
{
    SimControlElemnet::SimControlElemnet()
    {
    }
    SimControlElemnet::~SimControlElemnet()
    {
    }
    void SimControlElemnet::draw()
    {
        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("Simulator parameters");
        ImGui::Text("simulation method:");
        if (ImGui::BeginCombo("##combo", TYPE::sim_type_name[currentItem].c_str()))
        {
            for (int i = 0; i < TYPE::sim_type_name.size(); i++)
            {
                const bool isSelected = (currentItem == i);
                if (ImGui::Selectable(TYPE::sim_type_name[i].c_str(), isSelected))
                    currentItem = i;
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::Text("monitoring interval:");
        ImGui::SliderFloat("#Slider", &_duration, 0.0f, 1.0f);

        ImGui::Text("Field parameters: ");
        if (_visualize)
        {
            _simControl->doWatch(_duration);
        }
        // open Dialog Simple
        if (ImGui::Button("Open File"))
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".json", ".");
        // display
        std::string filePathName = "hairmodel.json";
        std::string filePath = "../scene/model/hairmodel.json";
        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
        {
            // action if OK
            if (ImGuiFileDialog::Instance()->IsOk())
            {
                filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::Text("Model File name:");
        ImGui::Text("%s", filePath.c_str());
        if (ImGui::Button("Start"))
        {
            _simControl = std::make_shared<SimControl>((TYPE::sim_type)currentItem);
            _simControl->doSim();
            _visualize = true;
        }
        ImGui::End();
    }

    SimControl::SimControl(TYPE::sim_type type)
    {
        _sim = std::make_shared<Simulator>(type);
        _watch = std::make_unique<SimWatch>(_sim);
        tick = glfwGetTime();
    }

    SimWatch::SimWatch(std::shared_ptr<Simulator> sim)
    {
        int width = sim->getFieldW();
        int height = sim->getFieldH();
        // create buffer
        _sim = sim;
        _ibuffer = std::make_unique<ImageBuffer>(width, height);
        _imglayer = std::make_unique<ImageDispalyLayer>();
    }

    SimWatch::~SimWatch()
    {
    }

    void SimWatch::update_buffer()
    {
        // copy image from simulation result
        // display image
        _sim->draw(_ibuffer);
        _imglayer->update_ImageBuffer(_ibuffer);
    }

    void SimWatch::imageView()
    {
        _imglayer->draw();
    }

    SimControl::~SimControl()
    {
        // todo thread control!!!
        doStop();
        _cal_thread.join();
    }

    void SimControl::doSim()
    {
        auto cal = [=]()
        {
            while (_run)
            {
                _sim->calc();
            }
        };
        _cal_thread = std::thread(cal);
    }

    void SimControl::doWatch(double imageDuration)
    {

        if (glfwGetTime() - tick > imageDuration)
        {
            _watch->update_buffer();
            tick = glfwGetTime();
        }
        _watch->imageView();
    }

    void SimControl::doStop()
    {
        _run = false;
    }
}
