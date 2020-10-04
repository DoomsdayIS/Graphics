#include <iostream>
#include <exception>
#include <fstream>
#include <cmath>
#include "picture.h"

void Picture::read(int argc, char **argv) {
    if (argc != 4 and argc != 6){
        throw std::runtime_error("Invalid arguments");
    }
    inputFile = argv[1];
    open();
    outputFile = argv[2];
    meta = atoi(argv[3]);
    if (meta < 0 or meta > 3){
        throw std::runtime_error("Error with meta!");
    }
    if (meta == 0){
        offset = atoi(argv[4]);
        if (offset < - 255 or offset > 255){
            throw std::runtime_error("Error with offset!");
        }
        factor = atof(argv[5]);
        if (factor <= 0 or factor > 255){
            throw std::runtime_error("Error with factor!");
        }
        oper0();
    } else if (meta == 1){
        offset = atoi(argv[4]);
        if (offset < - 255 or offset > 255){
            throw std::runtime_error("Error with offset!");
        }
        factor = atof(argv[5]);
        if (factor <= 0 or factor > 255){
            throw std::runtime_error("Error with factor!");
        }
        oper1();
    } else if (meta == 2){
        oper2();
    } else if (meta == 3){
        oper3();
    }
    write();
}

void Picture::open() {
    std::ifstream input(inputFile, std::ios_base::in | std::ios_base::binary);
    if (!input.is_open ()) {
        throw std::runtime_error ("Input file wasn't open");
    }
    input >> tmpMode >> width >> height >> color;
    if (input.fail ()) {
        throw std::runtime_error ("Input file failed");
    }
    if (tmpMode != "P6" and tmpMode != "P5") {
        throw std::runtime_error ("Incorrect mode of PGM ,should be P6");
    }
    if (tmpMode == "P6"){
        data1 = new Pixel[width * height];
        char partPixel;
        char* buffer = &partPixel;
        input.read (buffer, 1);
        for (int i = 0; i < width * height; ++i) {
            input.read (buffer, 1);
            data1[i].First = *buffer;
            input.read (buffer, 1);
            data1[i].Second = *buffer;
            input.read (buffer, 1);
            data1[i].Third = *buffer;
        }
        input.close ();
    }
    if (tmpMode == "P5"){
        data2 = new uchar[width * height];
        char partPixel;
        char* buffer = &partPixel;
        input.read (buffer, 1);
        input.read((char*)(&data2[0]), width * height);
        input.close ();
    }
}

void Picture::oper0() {
    if (tmpMode == "P6"){
        for (int i = 0; i < width*height; ++i) {
                double buffer = gamma_remove((double) data1[i].First);
                buffer = (buffer - offset)*factor;
                buffer = fmax(0,buffer);
                buffer = fmin(buffer,255);
                data1[i].First = round ((int) (gamma_set(buffer)));
                buffer = gamma_remove((double) data1[i].Second);
                buffer = (buffer - offset)*factor;
                buffer = fmax(0,buffer);
                buffer = fmin(buffer,255);
                data1[i].Second = round ((int) (gamma_set(buffer)));
                buffer = gamma_remove((double) data1[i].Third);
                buffer = (buffer - offset)*factor;
                buffer = fmax(0,buffer);
                buffer = fmin(buffer,255);
                data1[i].Third = round ((int) (gamma_set(buffer)));
        }
    }
    if (tmpMode == "P5"){
        for (int i = 0; i < width*height; ++i) {
            double buffer = gamma_remove((double)data2[i]);
            buffer = (buffer - offset)*factor;
            buffer = fmax(0,buffer);
            buffer = fmin(buffer,255);
            data2[i] = round((int)gamma_set(buffer));
        }
    }
}

void Picture::oper1() {
    is_rgb = false;
    for (int i = 0; i < width*height; ++i) {
        double buffer = gamma_remove((double) data1[i].First);
        buffer = (buffer - offset)*factor;
        buffer = fmax(0,buffer);
        buffer = fmin(buffer,255);
        data1[i].First = round ((int) (gamma_set(buffer)));
    }
}

void Picture::oper2() {
    double all_max,all_min;
    all_min = 255;
    all_max = 0;
    if (tmpMode == "P6"){
        for (int j = 0; j < width*height; ++j) {
            double buffer = gamma_remove((double) data1[j].First);
            if (buffer > all_max){
                all_max = buffer;
            }
            if (buffer < all_min){
                all_min = buffer;
            }
            buffer = gamma_remove((double) data1[j].Second);
            if (buffer > all_max){
                all_max = buffer;
            }
            if (buffer < all_min){
                all_min = buffer;
            }
            buffer = gamma_remove((double) data1[j].Third);
            if (buffer > all_max){
                all_max = buffer;
            }
            if (buffer < all_min){
                all_min = buffer;
            }
        }
        for (int i = 0; i < width*height; ++i) {
            double buffer = gamma_remove((double) data1[i].First);
            buffer = (buffer - all_min)*255/(all_max-all_min);
            buffer = fmax(0,buffer);
            buffer = fmin(buffer,255);
            data1[i].First = round ((int) (gamma_set(buffer)));
            buffer = gamma_remove((double) data1[i].Second);
            buffer = (buffer - all_min)*255/(all_max-all_min);
            buffer = fmax(0,buffer);
            buffer = fmin(buffer,255);
            data1[i].Second = round ((int) (gamma_set(buffer)));
            buffer = gamma_remove((double) data1[i].Third);
            buffer = (buffer - all_min)*255/(all_max-all_min);
            buffer = fmax(0,buffer);
            buffer = fmin(buffer,255);
            data1[i].Third = round ((int) (gamma_set(buffer)));
        }
    }
    if (tmpMode == "P5"){
        for (int i = 0; i < width*height; ++i) {
            double buffer = gamma_remove((double)data2[i]);
            if (buffer > all_max){
                all_max = buffer;
            }
            if (buffer < all_min){
                all_min = buffer;
            }
        }
        for (int i = 0; i < width*height; ++i) {
            double buffer = gamma_remove((double)data2[i]);
            buffer = (buffer - all_min)*255/(all_max-all_min);
            buffer = fmax(0,buffer);
            buffer = fmin(buffer,255);
            data2[i] = round((int)gamma_set(buffer));
        }
    }
    std::cout << all_min << " " << all_max << std::endl;
}

void Picture::oper3() {
    is_rgb = false;
    double all_max,all_min;
    all_min = 255;
    all_max = 0;
    for (int i = 0; i < width*height; ++i) {
        double buffer = gamma_remove((double) data1[i].First);;
        if (buffer > all_max){
            all_max = buffer;
        }
        if (buffer < all_min){
            all_min = buffer;
        }
    }
    for (int i = 0; i < width*height; ++i) {
        double buffer = gamma_remove((double) data1[i].First);
        buffer = (buffer - all_min)*255/(all_max-all_min);
        buffer = fmax(0,buffer);
        buffer = fmin(buffer,255);
        data1[i].First = round ((int) (gamma_set(buffer)));
    }
    std::cout << all_min << " "<< 255/(all_max - all_min) << std::endl;

}

double Picture::gamma_set(double value) {
    value = value / 255;
    if (tmpMode == "P6" and is_rgb) {
        if (value < 0.0031308)
            return value * 12.92 * 255.;
        else
            return 255. * (1.055*pow(value,1/2.4) - 0.055);
    }
    else {
        return 255. * pow(value, 1 / 2.2);
    }
}

double Picture::gamma_remove(double value) {
    value = value / 255;
    if (tmpMode == "P6" and is_rgb) {
        if (value < 0.04045)
            return 255. * value / 12.92;
        else
            return 255. * pow(((value+ 0.055)/1.055),2.4);
    }
    else {
        return 255. * pow (value, 2.2);
    }
}

void Picture::write() {
    std::ofstream fout(outputFile, std::ios_base::binary);
    if (!fout.is_open()){
        throw std::runtime_error("Incorrect Output file name!");
    }
    fout << tmpMode << '\n';
    fout << width << ' ' << height << '\n';
    fout << color << '\n';
    if (tmpMode == "P5"){
        fout.write((char*)&data2[0],width*height);
    }
    if (tmpMode == "P6"){
        fout.write((char*)&data1[0],width*height*3);
    }
    if (fout.fail ()) {
        throw std::runtime_error ("Input file failed");
    }
    fout.close();
}
