#ifndef IMAGE_BUFFER
#define IMAGE_BUFFER
#include "Shader.hpp"
#include <memory>
#include "GUIElement.hpp"

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
#endif