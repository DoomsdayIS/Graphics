#include <iostream>
#include <fstream>
#include <string>
using namespace std;

typedef unsigned char uchar;


struct RGB{
    uchar R;
    uchar G;
    uchar B;
};

class PNMfile{
    char format[2];
    char space;
    int width, height, deep;
    uchar* picture;
    RGB* pictureRGB;
    void readP5(ifstream& fin){
        uchar* pic = new uchar;
        fin.read((char*)pic, width*height - 1);
        if (fin.eof()){
            cout << "Хай, у тебя картинка битая";
            fin.close();
            exit(4);
        }
        fin.read((char*)pic, 1);
        picture = pic;
    }
    void readP6(ifstream& fin){
        RGB* picRGB = new RGB;
        fin.read((char*)picRGB,width*height*3 - 3);
        if (fin.eof()){
            cout << "Хай, у тебя картинка битая";
            fin.close();
            exit(4);
        }
        fin.read((char*)picRGB,3);
        pictureRGB = picRGB;
    }
    void writeP5(ofstream& fout){
        fout.write((char*)picture,width*height);
        delete [] picture;
    }
    void writeP6(ofstream& fout){
        fout.write((char*)pictureRGB,width*height*3);
        delete [] pictureRGB;
    }
    void inversionP5(){
        for (int i = 0; i < width*height; ++i) {
            picture[i] = deep - picture[i];
        }
    }
    void inversionP6(){
        for (int i = 0; i < width*height; ++i) {
            pictureRGB[i].R = deep - pictureRGB[i].R;
            pictureRGB[i].G = deep - pictureRGB[i].G;
            pictureRGB[i].B = deep - pictureRGB[i].B;
        }
    }
    void h_mirrorP5(){
        uchar* pic = new uchar[width*height];
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                pic[i*width + j] = picture[width*(height - i - 1) + j];
            }
        }
        delete [] picture;
        picture = pic;
    }
    void h_mirrorP6(){
        RGB* picRGB = new RGB[width*height];
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                picRGB[i*width + j] = pictureRGB[width*(height - i - 1) + j];
            }
        }
        delete [] pictureRGB;
        pictureRGB = picRGB;
    }
    void v_mirrorP5(){
        uchar* pic = new uchar[width*height];
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                pic[i*width + j] = picture[width*i + (width - 1 - j)];
            }
        }
        delete [] picture;
        picture = pic;
    }
    void v_mirrorP6(){
        RGB* picRGB = new RGB[width*height];
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                picRGB[i*width + j] = pictureRGB[width*i + (width - 1 - j)];
            }
        }
        delete [] pictureRGB;
        pictureRGB = picRGB;
    }
    void cwP5(){
        uchar* pic = new uchar[width*height];
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                pic[i*height + j] = picture[width*(height - j - 1) + i];
            }
        }
        delete [] picture;
        picture = pic;
        swap(width,height);
    }
    void cwP6(){
        RGB* picRGB = new RGB[width*height];
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                picRGB[i*height + j] = pictureRGB[width*(height - j - 1) + i];
            }
        }
        delete [] pictureRGB;
        pictureRGB = picRGB;
        swap(width,height);
    }
    void ccwP5(){
        uchar* pic = new uchar[width*height];
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                pic[i*height + j] = picture[width - i - 1 + j*width];
            }
        }
        delete [] picture;
        picture = pic;
        swap(width,height);
    }
    void ccwP6(){
        RGB* picRGB = new RGB[width*height];
        for (int i = 0; i < width; ++i) {
            for (int j = 0; j < height; ++j) {
                picRGB[i*height + j] = pictureRGB[width - i - 1 + j*width];
            }
        }
        delete [] pictureRGB;
        pictureRGB = picRGB;
        swap(width,height);
    }
public:
    void read(ifstream& fin) {
        fin >> format >> width >> height >> deep;
        fin.read(&space, 1);
        if(strncmp(format,"P6",2) != 0 and  strncmp(format,"P5",2) != 0){
            cout << "Неверный формат изображения";
            fin.close();
            exit(1);
        }
        if (deep < 0 or deep > 255){
            cout << "Неверная глубина изображения";
            fin.close();
            exit(2);
        }
        if (width < 1 or height < 1){
            cout << "Неверные размеры изображения";
            fin.close();
            exit(3);
        }
        if (format[1] == '6') {
            readP6(fin);
        } else {
            readP5(fin);
        }
    }
    void write(ofstream& fout){
        fout << format << '\n' << width << ' ' << height << '\n' << deep << '\n';
        if (format[1] == '6') {
            writeP6(fout);
        } else {
            writeP5(fout);
        }
    }
    void inversion(ifstream& fin,ofstream& fout){
        read(fin);
        if (format[1] == '6'){
            inversionP6();
        } else{
            inversionP5();
        }
        write(fout);
    }
    void horizontal_mirror(ifstream& fin,ofstream& fout){
        read(fin);
        if (format[1] == '6'){
            h_mirrorP6();
        }else{
            h_mirrorP5();
        }
        write(fout);
    }
    void vertical_mirror(ifstream& fin,ofstream& fout){
        read(fin);
        if (format[1] == '6'){
            v_mirrorP6();
        }else{
            v_mirrorP5();
        }
        write(fout);
    }
    void counterclockwise(ifstream& fin,ofstream& fout){
        read(fin);
        if (format[1] == '6'){
            ccwP6();
        }else{
            ccwP5();
        }
        write(fout);
    }
    void clockwise(ifstream& fin,ofstream& fout){
        read(fin);
        if (format[1] == '6'){
            cwP6();
        }else{
            cwP5();
        }
        write(fout);
    }
};

int main(int argc,char* argv[]) {
    if (argc != 4){
        cout << "Неверный формат ввода!";
    }else{
        ifstream fin(argv[1],ios_base::binary);
        ofstream fout(argv[2]);
        if ( fin.is_open() and fout.is_open()){
            if (atoi(argv[3]) == 0 or atoi(argv[3]) == 1 or atoi(argv[3]) == 2 or atoi(argv[3]) == 3 or atoi(argv[3]) == 4){
                PNMfile pnm;
                if (atoi(argv[3]) == 0)
                    pnm.inversion(fin,fout);
                if (atoi(argv[3]) == 1)
                    pnm.vertical_mirror(fin,fout);
                if (atoi(argv[3]) == 2)
                    pnm.horizontal_mirror(fin,fout);
                if (atoi(argv[3]) == 3)
                    pnm.clockwise(fin,fout);
                if (atoi(argv[3]) == 4)
                    pnm.counterclockwise(fin,fout);
            } else{
                cout << "Неверная операция!";
            }
        }else{
            cout << "Неверная директория файлов!";
        }
        fin.close();
        fout.close();
    }
    return 0;
}
