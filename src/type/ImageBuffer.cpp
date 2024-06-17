#include "ImageBuffer.hpp"
#include <glad/glad.h>

#include "imgui.h"
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
    void ImageBuffer::flipY()
    {
        unsigned char *temp = new unsigned char[_w * _h * 3];
        for (int i = 0; i < _h; i++)
        {
            for (int j = 0; j < _w; j++)
            {
                temp[3 * (j + i * _w) + 0] = _img[3 * (j + (_h - i - 1) * _w) + 0];
                temp[3 * (j + i * _w) + 1] = _img[3 * (j + (_h - i - 1) * _w) + 1];
                temp[3 * (j + i * _w) + 2] = _img[3 * (j + (_h - i - 1) * _w) + 2];
            }
        }
        delete[] _img;
        _img = temp;
    }

}