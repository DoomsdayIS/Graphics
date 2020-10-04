#ifndef GRAPHICS_PICTURE_H
#define GRAPHICS_PICTURE_H

typedef unsigned char uchar;
#include <string>


class Pixel {
public:
    uchar First;
    uchar Second;
    uchar Third;
};

class Picture {
public:
    void read(int argc, char* argv[]);
private:
    bool is_rgb = true;
    void write();
    void open();
    void oper0();
    void oper1();
    void oper2();
    void oper3();
    double gamma_set(double);
    double gamma_remove(double);
    std::string inputFile,outputFile,tmpMode;
    int width;
    int height;
    int color;
    int meta;
    int offset;
    double factor;
    Pixel* data1;
    uchar* data2;
};


#endif //GRAPHICS_PICTURE_H
