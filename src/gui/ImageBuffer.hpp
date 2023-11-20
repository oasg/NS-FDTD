#ifndef IMAGE_BUFFER
#define IMAGE_BUFFER
#include "Shader.hpp"
#include <memory>

namespace GUI
{
    class ImageBuffer
    {
    private:
        unsigned char *_img;  //8bit rgb
        int _w,_h;

    public:
        ImageBuffer(int width,int hight);
        void Write(int x,int y,unsigned char r,unsigned char g,unsigned char b);
        void ColorBlend(int x,int y,unsigned char r,unsigned char g,unsigned char b);
        int getWidth(){return _w;}
        int getHeight(){return _h;}
        ~ImageBuffer();
        unsigned char *getImg(){return _img;}
    };

    const float texCoords[] = {
        // positions              // texture coords
        0.5f,  0.5f, 0.0f,      1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,      1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,      0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,      0.0f, 1.0f    // top left 
    };
    const unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    class ImageDispalyLayer{
        public:
            ImageDispalyLayer();
            ~ImageDispalyLayer();
            void update_ImageBuffer(ImageBuffer &img);
            void draw_ImageLayer();
            void upadte_ratio();
            GLuint get_texture(){return _texture;}
        private:
            unsigned int VAO, VBO, EBO;
            GLuint _texture;
            std::shared_ptr<Shader> _shader;
    };
}
#endif