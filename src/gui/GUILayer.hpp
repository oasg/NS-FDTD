#ifndef GUILAYER_HPP
#define GUILAYER_HPP
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
namespace GUI{
    class GUILayer
    {
    private:
        /* data */
    public:
        GUILayer(GLFWwindow *_window);
        ~GUILayer();
        void GUI_Begin();
        void GUI_End();
        void GUI_Draw();

    };
    
    
    
}
#endif