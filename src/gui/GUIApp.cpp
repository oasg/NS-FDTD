#include "GUIApp.hpp"
#include "Menu.hpp"
#include "SimControl.hpp"
namespace GUI
{

    GUIApp::GUIApp(GLFWwindow *_window)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(_window, true);
        const char *glsl_version = "#version 330";
        ImGui_ImplOpenGL3_Init(glsl_version);
        _gui_elements.push_back(std::make_shared<MenuElement>());
        _gui_elements.push_back(std::make_shared<SimControlElemnet>());
    }

    GUIApp::~GUIApp()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void GUIApp::GUI_Begin()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void GUIApp::GUI_PostRender()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    void GUIApp::GUI_Draw()
    {
        for(auto ep: _gui_elements){
            ep->draw();
        }

    }
}