#include "Window.hpp"
#include <iostream>
#include <thread>

namespace GUI
{
    GUIWindow::GUIWindow(int16_t width, int16_t height)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        _window = glfwCreateWindow(width, height, "EMQuest", nullptr, nullptr);
        if (_window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            glfwTerminate();
        }
    }
    GUIWindow::~GUIWindow()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }
    void GUIWindow::InitContext()
    {
        _guilayer = std::make_unique<GUIApp>(_window);
    }
    void GUIWindow::doLoop()
    {
        double tick = glfwGetTime();
        while (!glfwWindowShouldClose(_window))
        {
            std::chrono::milliseconds sleepDuration(33); // 30fps
            std::this_thread::sleep_for(sleepDuration);
            glClear(GL_COLOR_BUFFER_BIT);
            _guilayer->GUI_Begin();
            _guilayer->GUI_Draw();
            _guilayer->GUI_PostRender();
            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }
}