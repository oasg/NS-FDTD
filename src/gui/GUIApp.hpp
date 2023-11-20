#ifndef GUILAYER_HPP
#define GUILAYER_HPP
#include <vector>
#include "GUIElement.hpp"
#include "SimControl.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
namespace GUI{
    class GUIApp
    {

    public:
        explicit GUIApp(GLFWwindow *_window);
        ~GUIApp();
        void GUI_Begin();
        void GUI_PostRender();
        void GUI_Draw();
    private:
        std::vector<std::shared_ptr<GUIElement>> _gui_elements;
    };

}
#endif