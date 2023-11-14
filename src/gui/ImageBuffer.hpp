#ifndef IMAGE_BUFFER
#define IMAGE_BUFFER
namespace GUI
{
    class ImageBuffer
    {
    private:
        char *_img;
        int _w,_h;
    public:
        ImageBuffer(int width,int hight);
        void Write(int x,int y,int r,int g,int b);
        void ColorBlend(int x,int y,int r,int g,int b);
        int getWidth(){return _w;}
        int getHeight(){return _h;}
        ~ImageBuffer();
        char *getImg(){return _img;}
    };
}
#endif