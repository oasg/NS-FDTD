#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Simulator.h"
#include "ImageBuffer.hpp"
#include "GUILayer.hpp"
namespace GUI
{
    class GUIWindow
    {
    private:
        /* data */
        GLFWwindow *_window;
        bool isEnable = true;
        std::unique_ptr<GUILayer> _guilayer;
    public:
        GUIWindow(int16_t width = 200, int16_t height = 200);
        ~GUIWindow();
        void InitContext();
        void doLoop(ImageBuffer &img);
        GLFWwindow *GetWindow(){ return _window; };
        //virtual void onRender() = 0;
    };
    
}

#endif