#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
typedef unsigned char uchar;

using namespace std;

struct Point{
    double x = 0;
    double y = 0;
};

class PNM_DrawLine{
public:
    void draw(int argc, char* argv[]){
        if (argc < 9 or argc > 10) {
            cerr << "Incorrect count of arguments\n";
            exit(1);
        }
        string inputFileName = argv[1];
        string outputFileName = argv[2];
        short brush_color = (short)stoi(argv[3]);
        double thick = stof(argv[4]);
        Point start;
        Point end;
        start.x = stod(argv[5]);
        start.y = stod(argv[6]);
        end.x = stod(argv[7]);
        end.y = stod(argv[8]);
        //
        read(inputFileName);
        Wu(start, end, brush_color, thick);
        write(outputFileName);
        //
    }
private:
    //
    char P{};
    short type_of_pnm = -1, colors = -1;
    int width = -1, height = -1;
    vector<uchar> buffer;
    //
    void read(const string& fileName){
        ifstream fin(fileName,ios_base::binary);
        if (!fin.is_open()) {
            cerr << "Incorrect name of file\n";
            exit(1);
        }
        fin >> P >> type_of_pnm >> width >> height >> colors;

        if (P != 'P') {
            cerr << "Error format : (not P)\n";
            exit(1);
        }
        if (type_of_pnm != 5) {
            cerr << "Error format : (not P5 or P6)\n";
            exit(1);
        }
        if (colors != 255) {
            cerr << "Error format of brush_color";
            exit(1);
        }
        //
        fin.get();
        buffer.clear();
        buffer.resize(width * height);
        fin.read((char *) &buffer[0], width * height);
        fin.close();
        //
    }
    void write(const string& fileName) {
        ofstream fout(fileName, ios_base::binary);
        if (!fout.is_open()) {
            cerr << "Output file wasn't open\n";
            exit(1);
        }
        //
        fout << "P" << type_of_pnm << '\n';
        fout << width << ' ' << height << '\n';
        fout << colors << '\n';
        fout.write((char*) &buffer[0], width * height);
        fout.close();
        //
    }

    static double dest(Point a, Point b) {
        return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    }

    void Wu(Point start, Point end, uchar brush_color, double thick){
        if (thick <= 0)
            return;
        bool steep = abs(end.y - start.y) > abs(end.x - start.x);
        if (steep) {
            swap(start.x, start.y);
            swap(end.x, end.y);
        }
        if (start.x > end.x) {
            swap(start.x, end.x);
            swap(start.y, end.y);
        }

        double dx = end.x - start.x;
        double dy = end.y - start.y;
        double grad = dy / dx;
        double y = start.y + grad * (round(start.x) - start.x);

        for(int cur_step_x = (int)round(start.x); cur_step_x <= (int)round(end.x); cur_step_x++) {
            int dest_to_edge = (int)(y - (thick - 1) / 2);
            for (int cur_step_y = dest_to_edge; cur_step_y <= (int)(dest_to_edge + thick); cur_step_y++) {
                double bright = min(1.0, (thick + 1.0) / 2.0 - fabs(y - cur_step_y));
                if (steep)
                    drawPoint(cur_step_y, cur_step_x, 1.0 - bright, brush_color);
                else
                    drawPoint(cur_step_x, cur_step_y, 1.0 - bright, brush_color);
            }
            y += grad;
        }

        Point p_start = {round(start.x), round(start.y)};
        for (int p_X = (int)(round(start.x) - thick / 2); p_X < round(start.x); p_X++) {
            y = start.y + grad * (p_X - start.x);
            for (int p_Y = int(y - (thick - 1) / 2.0); p_Y <= int(y - (thick - 1) / 2.0 + thick); p_Y++) {
                double minimum = min(1.0, (thick + 0.5) / 2.0 -
                                          dest({(float) p_X, (float) p_Y}, {p_start.x, p_start.y}));
                if (steep)
                    drawPoint(p_Y, p_X, 1.0 - minimum, brush_color);
                else
                    drawPoint(p_X, p_Y, 1.0 - minimum, brush_color);
            }
        }

        Point p_end = {round(end.x), round(end.y)};
        for (int p_X = (int)round(end.x) + 1; p_X <= round(end.x) + thick / 2; p_X++) {
            y = start.y + grad * (p_X - start.x);
            for (int p_Y = int(y - (thick - 1) / 2.0); p_Y <= int(y - (thick - 1) / 2.0 + thick); p_Y++) {
                double minimum = min(1.0, (thick + 0.5) / 2.0 -
                                          dest({(float) p_X, (float) p_Y}, {p_end.x, p_end.y}));
                if (steep)
                    drawPoint(p_Y, p_X, 1.0 - minimum, brush_color);
                else
                    drawPoint(p_X, p_Y, 1.0 - minimum, brush_color);
            }
        }
    }

    void drawPoint(int x, int y, double darkness, uchar brush_color) {
        if (y < 0 || y >= height || x < 0 || x >= width)
            return;
        double transparency = max(min(darkness, 1.), 0.);
        double line_color = brush_color / 255.;
        double pic_with_gamma = buffer[width * y + x] / 255.;
        double pic_out_gamma;

        if (pic_with_gamma <= 0.04045) {
            pic_out_gamma = pic_with_gamma / 12.92;
        } else {
            pic_out_gamma = pow((pic_with_gamma + 0.055) / 1.055, 2.4);
        }
        double c = (1 - transparency) * line_color + transparency * pic_out_gamma;

        if (c <= 0.0031308)
            buffer[width * y + x] = 255. * 12.92 * c;
        else
            buffer[width * y + x] = 255. * (1.055 * pow(c, 1 / 2.4) - 0.055);
    }


};


int main(int argc,char* argv[]){
    //
    PNM_DrawLine pnm;
    pnm.draw(argc,argv);
    //
    return 0;
}

