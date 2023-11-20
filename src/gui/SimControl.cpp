#include "SimControl.hpp"
#include "imgui.h"
namespace GUI{
    SimControlElemnet::SimControlElemnet() {
       ftdt_items =  {"FTDT_TE","FTDT_TM","NsFTDT_TE","NsFTDT_TE"};
    }
    SimControlElemnet::~SimControlElemnet() {

    }
    void SimControlElemnet::draw() {
        // Create a window called "My First Tool", with a menu bar.
        ImGui::Begin("Simulator parameters");
        ImGui::Text("simulation method:");
        if (ImGui::BeginCombo("##combo", ftdt_items[currentItem].c_str())) {
            for (int i = 0;i<ftdt_items.size();i++) {
                const bool isSelected = (currentItem == i);
                if (ImGui::Selectable(ftdt_items[i].c_str(), isSelected))
                    currentItem = i;
                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }

        if (ImGui::Button("Start")){

        }
        ImGui::End();
    }




}

