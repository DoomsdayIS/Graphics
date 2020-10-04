#include "picture.h"

#include <fstream>
#include <cmath>
#include <iostream>

void Picture::ReadPPM (const std::string& inputFile) {
    std::ifstream input (inputFile, std::ios_base::in | std::ios_base::binary);
    if (!input.is_open ()) {
        throw std::runtime_error ("Input file wasn't open");
    }

    std::string tmpMode;
    input >> tmpMode >> width >> height >> color;
    if (input.fail ()) {
        throw std::runtime_error ("Input file failed");
    }
    if (tmpMode != "P6") {
        throw std::runtime_error ("Incorrect mode of PGM ,should be P6");
    }
    data = new Pixel[width * height];
    char partPixel;
    char* buffer = &partPixel;
    input.read (buffer, 1);
    for (int i = 0; i < width * height; ++i) {
        input.read (buffer, 1);
        data[i].First = *buffer;
        input.read (buffer, 1);
        data[i].Second = *buffer;
        input.read (buffer, 1);
        data[i].Third = *buffer;
    }
    input.close ();
}

void Picture::ReadAllPPM (std::string& inputFile) {
    inputFile.insert (inputFile.size () - 4, "_1");
    std::ifstream inputFirst (inputFile, std::ios_base::in | std::ios_base::binary);
    if (!inputFirst.is_open ()) {
        throw std::runtime_error ("Input file 1 wasn't open");
    }

    inputFile.replace (inputFile.size () - 5, 1, "2");
    std::ifstream inputSecond (inputFile, std::ios_base::in | std::ios_base::binary);
    if (!inputSecond.is_open ()) {
        throw std::runtime_error ("Input file 2 wasn't open");
    }

    inputFile.replace (inputFile.size () - 5, 1, "3");
    std::ifstream inputThird (inputFile, std::ios_base::in | std::ios_base::binary);
    if (!inputThird.is_open ()) {
        throw std::runtime_error ("Input file 3 wasn't open");
    }

    std::string mode1, mode2, mode3;
    inputFirst >> mode1;
    inputSecond >> mode2;
    inputThird >> mode3;
    if ((mode1 != "P5") || (mode2 != "P5") || (mode3 != "P5")) {
        throw std::runtime_error ("Wrong file format");
    }

    int width2, width3, height2, height3;
    inputFirst >> width >> height;
    inputSecond >> width2 >> height2;
    inputThird >> width3 >> height3;
    if ((width2 != width) || (height2 != height) || (width3 != width) || (height3 != height)) {
        throw std::runtime_error ("Wrong sizes");
    }

    int colorDepth1;
    inputFirst >> colorDepth1;
    if (colorDepth1 != 255) {
        throw std::runtime_error ("Wrong num of color 1 img.");
    }
    int colorDepth2;
    inputSecond >> colorDepth2;
    if (colorDepth2 != 255) {
        throw std::runtime_error ("Wrong num of color 2 img.");
    }
    int colorDepth3;
    inputThird >> colorDepth3;
    if (colorDepth3 != 255) {
        throw std::runtime_error ("Wrong num of color 3 img.");
    }

    char partPixel;
    char* buffer = &partPixel;

    inputFirst.read (buffer, 1);
    inputSecond.read (buffer, 1);
    inputThird.read (buffer, 1);
    data = new Pixel[width * height];


    for (int i = 0; i < width * height; ++i) {
        inputFirst.read (buffer, 1);
        data[i].First = *buffer;
        inputSecond.read (buffer, 1);
        data[i].Second = *buffer;
        inputThird.read (buffer, 1);
        data[i].Third = *buffer;
    }

    if (inputFirst.fail () || inputSecond.fail () || inputThird.fail ()) {
        throw std::runtime_error ("Input file failed");
    }
    inputFirst.close ();
    inputSecond.close ();
    inputThird.close ();
}

void Picture::WritePPM (const std::string& outputFile) {
    std::ofstream output (outputFile, std::ios_base::out | std::ios_base::binary);
    if (!output.is_open ()) {
        throw std::runtime_error ("Input file wasn't found");
    }
    output << "P6" << '\n';
    output << width << ' ' << height << '\n' << color << '\n';
    output.write ((char*) data, width * height * 3);
    if (output.fail ()) {
        throw std::runtime_error ("Input file failed");
    }
    output.close ();
}

void Picture::WritePGMS (std::string& outputFile) {
    std::ofstream output1, output2, output3;
    outputFile.insert (outputFile.size () - 4, "_1");
    output1.open (outputFile, std::ios_base::out | std::ios_base::binary);
    if (!output1.is_open ()) {
        throw std::runtime_error ("Input file 1 wasn't found");
    }
    outputFile.replace (outputFile.size () - 5, 1, "2");
    output2.open (outputFile, std::ios_base::out | std::ios_base::binary);
    if (!output2.is_open ()) {
        throw std::runtime_error ("Input file 2 wasn't found");
    }
    outputFile.replace (outputFile.size () - 5, 1, "3");
    output3.open (outputFile, std::ios_base::out | std::ios_base::binary);
    if (!output3.is_open ()) {
        throw std::runtime_error ("Input file 3 wasn't found");
    }

    output1 << "P5" << '\n';
    output1 << width << ' ' << height << '\n' << color << '\n';
    output2 << "P5" << '\n';
    output2 << width << ' ' << height << '\n' << color << '\n';
    output3 << "P5" << '\n';
    output3 << width << ' ' << height << '\n' << color << '\n';

    if (output1.fail () || output2.fail () || output3.fail ()) {
        throw std::runtime_error ("Output file failed");
    }

    for (int i = 0; i < width * height; i++) {
        output1.write ((char*) &data[i].First, 1);
        output2.write ((char*) &data[i].Second, 1);
        output3.write ((char*) &data[i].Third, 1);
    }
    output1.close ();
    output2.close ();
    output3.close ();
}

void Picture::ConvertFromRGB (const ColorSpace& Space) {
    if (space == Space) {
        return;
    }
    ConvertToRGB ();
    double Max, Min, H, S, L, C, V, M, Y, Cb, Cr, Co, Cg, Kr, Kg, Kb, R, G, B;
    switch (Space) {
        case RGB:
            break;
        case HSL:
        case HSV:
            for (int i = 0; i < width * height; ++i) {
                R = data[i].First / 255.0;
                G = data[i].Second / 255.0;
                B = data[i].Third / 255.0;
                Max = std::max (R, std::max (G, B));
                Min = std::min (R, std::min (G, B));
                V = Max;
                C = Max - Min;
                L = V - C / 2.0;
                if (C == 0) {
                    H = 0;
                } else {
                    if (V == R) {
                        H = (60.0) * ((G - B) / C);
                    }
                    else if (V == G) {
                        H = (60.0) * (2 + (B - R) / C);
                    }
                    else if (V == B) {
                        H = (60.0) * (4 + (R - G) / C);
                    }
                    else {
                        H = 0;
                    }
                }
                if (Space == HSV) {
                    S = (V == 0) ? 0 : C / V;
                    data[i].Third = (uchar) (V * 255.0);
                }
                if (Space == HSL) {
                    S = ((L == 0) || (L == 1)) ? 0 : ((V - L) / std::min (L, 1 - L));
                    data[i].Third = (uchar) (L * 255.0);
                }
                data[i].Second = (uchar) (S * 255.0);
                data[i].First = (uchar) ((H / 360.0) * 255.0);
            }
            space = Space;
            break;
        case YCbCr_601:
        case YCbCr_709:
            if (Space == YCbCr_601) {
                Kr = 0.299;
                Kg = 0.587;
                Kb = 0.114;
                space = YCbCr_601;
            }
            else {
                Kr = 0.0722;
                Kg = 0.2126;
                Kb = 0.7152;
                space = YCbCr_709;
            }
            for (int i = 0; i < width * height; ++i) {
                R = data[i].First / 255.0;
                G = data[i].Second / 255.0;
                B = data[i].Third / 255.0;
                Y = Kr * R + Kg * G + Kb * B;
                Cb = 0.5 * ((B - Y) / (1.0 - Kb));
                Cr = 0.5 * ((R - Y) / (1.0 - Kr));
                data[i].First = (uchar) (Y * 255.0);
                data[i].Second = (uchar) ((Cb + 0.5) * 255.0);
                data[i].Third = (uchar) ((Cr + 0.5) * 255.0);
            }
            break;
        case YCoCg:
            for (int i = 0; i < width * height; ++i) {
                R = data[i].First / 255.0;
                G = data[i].Second / 255.0;
                B = data[i].Third / 255.0;
                Y = R / 4 + G / 2 + B / 4;
                Co = R / 2 - B / 2;
                Cg = -R / 4 + G / 2 - B / 4;
                data[i].First = (uchar) (Y * 255.0);
                data[i].Second = (uchar) ((Co + 0.5) * 255.0);
                data[i].Third = (uchar) ((Cg + 0.5) * 255.0);
            }
            space = YCoCg;
            break;
        case CMY:
            for (int i = 0; i < width * height; ++i) {
                R = data[i].First / 255.0;
                G = data[i].Second / 255.0;
                B = data[i].Third / 255.0;
                C = 1 - R;
                M = 1 - G;
                Y = 1 - B;
                data[i].First = (uchar) (C * 255.0);
                data[i].Second = (uchar) (M * 255.0);
                data[i].Third = (uchar) (Y * 255.0);
            }
            space = CMY;
            break;
    }
}

void Picture::ConvertToRGB () {
    double H, S, L, C, H_D, X, m, R, G, B, Y, Cb, Cr, Co, Cg, M, Kr, Kg, Kb;
    switch (space) {
        case RGB:
            break;
        case HSL:
        case HSV:
            for (int i = 0; i < height * width; ++i) {
                H = (data[i].First / 255.0) * 360.0;
                S = data[i].Second / 255.0;
                L = data[i].Third / 255.0;
                H_D = H / 60;
                if (space == HSL) {
                    C = (1 - std::abs (2 * L - 1)) * S;
                    X = C * (1 - std::abs (fmod (H_D, 2) - 1));
                    m = L - C / 2.0;
                }
                else {
                    C = S * L;
                    X = C * (1.0 - std::abs (fmod (H_D, 2) - 1.0));
                    m = L - C;
                }

                m *= 255.0;
                if ((H_D >= 0) && (H_D <= 1)) {
                    data[i].First = (uchar) (C * 255.0 + m);
                    data[i].Second = (uchar) (X * 255.0 + m);
                    data[i].Third = (uchar) m;
                }
                if ((H_D > 1) && (H_D <= 2)) {
                    data[i].First = (uchar) (X * 255.0 + m);
                    data[i].Second = (uchar) (C * 255.0 + m);
                    data[i].Third = (uchar) m;
                }
                if ((H_D > 2) && (H_D <= 3)) {
                    data[i].First = (uchar) m;
                    data[i].Second = (uchar) (C * 255.0 + m);
                    data[i].Third = (uchar) (X * 255.0 + m);
                }
                if ((H_D > 3) && (H_D <= 4)) {
                    data[i].First = (uchar) m;
                    data[i].Second = (uchar) (X * 255.0 + m);
                    data[i].Third = (uchar) (C * 255.0 + m);
                }
                if ((H_D > 4) && (H_D <= 5)) {
                    data[i].First = (uchar) (X * 255.0 + m);
                    data[i].Second = (uchar) m;
                    data[i].Third = (uchar) (C * 255.0 + m);
                }
                if ((H_D > 5) && (H_D <= 6)) {
                    data[i].First = (uchar) (C * 255.0 + m);
                    data[i].Second = (uchar) m;
                    data[i].Third = (uchar) (X * 255.0 + m);
                }

            }
            break;
        case YCbCr_601:
        case YCbCr_709:
            if (space == YCbCr_601) {
                Kr = 0.299;
                Kg = 0.587;
                Kb = 0.114;
            }
            else {
                Kr = 0.0722;
                Kg = 0.2126;
                Kb = 0.7152;
            }
            for (int i = 0; i < width * height; ++i) {
                Y = data[i].First / 255.0;
                Cb = (data[i].Second / 255.0) - 0.5;
                Cr = (data[i].Third / 255.0) - 0.5;
                R = (Y + Cr * (2.0 - 2.0 * Kr));
                G = (Y - (Kb / Kg) * (2.0 - 2.0 * Kb) * Cb - (Kr / Kg) * (2.0 - 2.0 * Kr) * Cr);
                B = (Y + (2.0 - 2.0 * Kb) * Cb);
                if (R < 0) {
                    R = 0;
                }
                if (G < 0) {
                    G = 0;
                }
                if (B < 0) {
                    B = 0;
                }
                if (R > 1) {
                    R = 1;
                }
                if (G > 1) {
                    G = 1;
                }
                if (B > 1) {
                    B = 1;
                }
                data[i].First = (uchar) (R * 255.0);
                data[i].Second = (uchar) (G * 255.0);
                data[i].Third = (uchar) (B * 255.0);
            }
            break;
        case YCoCg:
            for (int i = 0; i < width * height; ++i) {
                Y = data[i].First / 255.0;
                Co = (data[i].Second / 255.0) - 0.5;
                Cg = (data[i].Third / 255.0) - 0.5;
                R = Y + Co - Cg;
                G = Y + Cg;
                B = Y - Co - Cg;
                if (R < 0) {
                    R = 0;
                }
                if (G < 0) {
                    G = 0;
                }
                if (B < 0) {
                    B = 0;
                }
                if (R > 1) {
                    R = 1;
                }
                if (G > 1) {
                    G = 1;
                }
                if (B > 1) {
                    B = 1;
                }
                data[i].First = (uchar) (R * 255.0);
                data[i].Second = (uchar) (G * 255.0);
                data[i].Third = (uchar) (B * 255.0);
            }
            break;
        case CMY:
            for (int i = 0; i < width * height; ++i) {
                C = data[i].First / 255.0;
                M = data[i].Second / 255.0;
                Y = data[i].Third / 255.0;
                R = 1 - C;
                G = 1 - M;
                B = 1 - Y;
                data[i].First = (uchar) (R * 255.0);
                data[i].Second = (uchar) (G * 255.0);
                data[i].Third = (uchar) (B * 255.0);
            }
            break;
    }
    space = RGB;
}