//
// Created by oasg on 2023/11/20.
//

#include "Menu.hpp"
#include "imgui.h"
namespace GUI{
    void MenuElement::draw() {
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        //ImGui::ShowDemoWindow();
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open"))
                {
                    // 处理Open菜单项点击事件
                }
                if (ImGui::MenuItem("Save"))
                {
                    // 处理Save菜单项点击事件
                }
                ImGui::EndMenu();
            }
            // 其他菜单项
            ImGui::EndMainMenuBar();
        }
    }
}
