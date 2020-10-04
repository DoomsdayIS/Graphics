#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>

using namespace std;
typedef unsigned char uchar;
#ifndef GRAPHICS_PNM_DITH_H
#define GRAPHICS_PNM_DITH_H

enum Dith_type{
    NONE = 0,
    ORDERED_8x8 = 1,
    RANDOM = 2,
    FLOYD_STEINBERG = 3,
    JARVIS_JUDICE_NINKE = 4,
    SIERRA_3 = 5,
    ATKINSON = 6,
    HALFTONE_4x4 = 7
};


class PNM_Dith {
private:
    char P;
    short type_of_pnm = -1, colors = -1, bitRate = -1;
    bool gradient;
    double gamma = 0;
    Dith_type type_of_dithering;
    int width = -1, height = -1;
    vector<uchar> buffer;
    //
    void read(const string& );
    void write(const string& );
    void none();
    void ordered();
    void random();
    void floyd();
    void jarvis();
    void sierra();
    void atkinson();
    void halftone();
    void fill_gradient();
    double gamma_set(double);
    double gamma_remove(double);
    int findNear(int);
    //
public:
    //
    void dithering(int argc,char* argv[]);
    //
};


#endif //GRAPHICS_PNM_DITH_H
