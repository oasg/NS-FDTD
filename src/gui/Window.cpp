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
        _window = glfwCreateWindow(width, height, "NS-FDTD", nullptr, nullptr);
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
        _imglayer = std::make_unique<ImageDispalyLayer>();
    }
    void GUIWindow::doLoop()
    {
        double tick = glfwGetTime();
        while (!glfwWindowShouldClose(_window))
        {
            std::chrono::milliseconds sleepDuration(33); // 30fps
            std::this_thread::sleep_for(sleepDuration);
            glClear(GL_COLOR_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            _guilayer->GUI_Begin();
//            _guilayer->GUI_DrawBuffer();
//            ImGui::Begin("Simulation watch");
//            ImVec2 vMin = ImGui::GetWindowContentRegionMin();
//			ImVec2 vMax = ImGui::GetWindowContentRegionMax();
//            int height = vMax.y - vMin.y;
//            int width = vMax.x - vMin.x;
//            int min = std::min(width, height);
//            ImGui::Image((void*)(intptr_t)_imglayer->get_texture(), ImVec2(min,min));
//            ImGui::End();
            _guilayer->GUI_Draw();
            _guilayer->GUI_PostRender();
            // when timer is up
            if (glfwGetTime() - tick > _imageDuration)
            {
                // copy image from simulation result
                // display image
//                sim->draw(img);
//                _imglayer->update_ImageBuffer(img);
                // reset the timer
                tick = glfwGetTime();
            }
            // _imglayer->upadte_ratio();
            // _imglayer->draw_ImageLayer();
            

            glfwSwapBuffers(_window);
            glfwPollEvents();
        }
    }
}