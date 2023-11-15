#include "ImageBuffer.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace GUI
{
    ImageBuffer::ImageBuffer(int width, int hight) : _w(width), _h(hight)
    {
        _img = new unsigned char[width * hight * 3];
    }
    void ImageBuffer::Write(int x, int y, unsigned char r,unsigned char g,unsigned char b)
    {
        _img[3 * (x + y * _w) + 0] = r;
        _img[3 * (x + y * _w) + 1] = g;
        _img[3 * (x + y * _w) + 2] = b;
    }
    void ImageBuffer::ColorBlend(int x, int y, unsigned char r,unsigned char g,unsigned char b)
    {
        _img[3 * (x + y * _w) + 0] =_img[3 * (x + y * _w) + 0]*0.4 + r * 0.6;
        _img[3 * (x + y * _w) + 1] =_img[3 * (x + y * _w) + 1]*0.4 + g * 0.6;
        _img[3 * (x + y * _w) + 2] =_img[3 * (x + y * _w) + 2]*0.4 + b * 0.6;
    }
    ImageBuffer::~ImageBuffer()
    {
        delete[] _img;
    }
    ImageDispalyLayer::ImageDispalyLayer()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(texCoords), indices, GL_STATIC_DRAW);

        glGenTextures(1, &_texture);
        glBindTexture(GL_TEXTURE_2D, _texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // pos.x pos.y pos.z
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0 * sizeof(float)));
        glEnableVertexAttribArray(0);
        // texcoord.x texcoord.y
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        _shader = std::make_shared<Shader>("../scene/shaders/Image_vert.glsl", "../scene/shaders/Image_frag.glsl");
        _shader->use();
        glUniform1i(glGetUniformLocation(_shader->ID, "Texture"), 0);
    }
    ImageDispalyLayer::~ImageDispalyLayer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
    void ImageDispalyLayer::update_ImageBuffer(ImageBuffer &img)
    {
        glActiveTexture(GL_TEXTURE0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.getWidth(), img.getHeight(), 0,GL_RGB, GL_UNSIGNED_BYTE, img.getImg());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void ImageDispalyLayer::draw_ImageLayer() {
        glBindTexture(GL_TEXTURE_2D, _texture);
        _shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    void ImageDispalyLayer::upadte_ratio()
    {
        int width,height;
        glfwGetWindowSize(glfwGetCurrentContext(),&width, &height);
        glUniform1f(glGetUniformLocation(_shader->ID, "aspect_ratio"), (float)height/(float)width);
    }
}