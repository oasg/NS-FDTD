#include"ImageBuffer.hpp"
namespace GUI{
    ImageBuffer::ImageBuffer(int width,int hight):_w(width),_h(hight)
    {
        _img = new char[width*hight*3];
    }
    void ImageBuffer::Write(int x, int y, int r, int g, int b)
    {
        _img[3*(x+y*_w)+0] = r;
        _img[3*(x+y*_w)+1] = g;
        _img[3*(x+y*_w)+2] = b;
    }
    void ImageBuffer::ColorBlend(int x, int y, int r, int g, int b)
    {
        _img[3*(x+y*_w)+0] += r*0.6;
        _img[3*(x+y*_w)+1] += g*0.6;
        _img[3*(x+y*_w)+2] += b*0.6;
    }
    ImageBuffer::~ImageBuffer()
    {
        delete[] _img;
    }
}