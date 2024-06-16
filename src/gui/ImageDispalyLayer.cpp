#include "ImageDispalyLayer.hpp"
#include "imgui.h"
namespace GUI{
    ImageDispalyLayer::ImageDispalyLayer()
    {

        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    ImageDispalyLayer::~ImageDispalyLayer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    void ImageDispalyLayer::update_ImageBuffer(std::shared_ptr<ImageBuffer> img)
    {
        //flip　y axis

        glActiveTexture(GL_TEXTURE0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->getWidth(), img->getHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, img->getImg());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void ImageDispalyLayer::draw() {
        ImGui::Begin("Simulation watch");
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();
        int height = vMax.y - vMin.y;
        int width = vMax.x - vMin.x;
        int min = std::min(width, height);
        ImGui::Image((void*)(intptr_t)_texture, ImVec2(min,min));
        ImGui::End();
    }
}
