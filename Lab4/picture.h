#ifndef GRAPHICS_PICTURE_H
#define GRAPHICS_PICTURE_H

#include <string>
typedef unsigned char uchar;

enum ColorSpace {
    RGB,
    HSL,
    HSV,
    YCbCr_601,
    YCbCr_709,
    YCoCg,
    CMY
};

class Pixel {
public:
    uchar First;
    uchar Second;
    uchar Third;
};

class Picture {
public:
    Picture () : space (RGB), width (0), height (0), color (255), data (nullptr) {};
    ~Picture () { delete[] data;};
    void SetColorSpace (const ColorSpace& Space) {space = Space;};
    void ReadPPM (const std::string&);
    void ReadAllPPM (std::string&);
    void ConvertFromRGB (const ColorSpace&);
    void WritePPM (const std::string&);
    void WritePGMS(std::string&);
private:
    ColorSpace space;
    int width;
    int height;
    int color;
    Pixel* data;
    void ConvertToRGB ();
};


#endif //GRAPHICS_PICTURE_H
