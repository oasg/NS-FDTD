#pragma once
#include "type/ImageBuffer.hpp"
#include "Shader.hpp"
#include <GLFW/glfw3.h>
namespace GUI{
class ImageDispalyLayer{
    public:
        ImageDispalyLayer();
        ~ImageDispalyLayer();
        void update_ImageBuffer(std::shared_ptr<ImageBuffer> img);
        GLuint get_texture(){return _texture;}
        void draw();
    private:
        unsigned int VAO, VBO, EBO;
        GLuint _texture;
        std::shared_ptr<Shader> _shader;

};
}
