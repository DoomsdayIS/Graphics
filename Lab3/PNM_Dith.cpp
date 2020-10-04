#include "PNM_Dith.h"

vector<vector<double>> ordered_l = {{0,48,12,60,3,51,15,63},
                               {32,16,44,28,35,19,47,31},
                               {8,56,4,52,11,59,7,55},
                               {40,24,36,2043,27,39,23},
                               {2,50,14,62,1,49,13,61},
                               {34,18,46,30,33,17,45,29},
                               {10,58,6,54,9,57,5,53},
                               {42,26,38,22,41,25,37,21}};

vector<vector<double>> JJN = {{0, 0, 0, 7, 5},
                          {3, 5, 7, 5, 3},
                          {1, 3, 5, 3, 1}};

vector<vector<double>> Sierra3 = {{0, 0, 0, 5, 3},
                              {2, 4, 5, 4, 2},
                              {0, 2, 3, 2, 0}};

vector<vector<double>> Atkinson = {{0, 0, 0, 1, 1},
                               {0, 1, 1, 1, 0},
                               {0, 0, 1, 0, 0}};

vector<vector<double>> Halftone = {{13.0, 11.0, 4.0 ,  8.0 },
                               {6.0 ,  0, 3.0 ,  15.0},
                               {14.0, 1.0 ,  2.0,  7.0},
                               {9.0,  5.0,  10.0, 12.0},
};

void PNM_Dith::read(const string &FileName) {
    ifstream fin(FileName,ios_base::binary);
    if (!fin.is_open()){
        cerr << "Incorrect name of Input file!";
        exit(1);
    }
    fin >> P >> type_of_pnm >> width >> height >> colors;
    if (P != 'P') {
        cerr << "Error format\n";
        exit(1);
    }
    if (type_of_pnm != 5) {
        cerr << "Error format\n";
        exit(1);
    }
    if (colors != 255) {
        cerr << "Error format of brush_color";
        exit(1);
    }
    fin.get();
    buffer.clear();
    buffer.resize(width*height);
    fin.read((char*)&buffer[0],width*height);
    fin.close();
}

void PNM_Dith::write(const string &FileName) {
    ofstream fout(FileName, ios_base::binary);
    if (!fout.is_open()){
        cerr << "Output file wasn't open\n";
        exit(1);
    }
    fout << "P" << type_of_pnm << '\n';
    fout << width << ' ' << height << '\n';
    fout << colors << '\n';

    fout.write((char*) &buffer[0], width * height);

    fout.close();
}

void PNM_Dith::none() {
    for (int i = 0; i < width * height; ++i) {
        int buff = (int)round(gamma_remove((double) buffer[i]));
        buff = findNear(buff);
        buffer[i] = round ((int) (gamma_set((double)buff)));
    }
}

void PNM_Dith::ordered() {
    for (auto& i:ordered_l){
        for (auto& j:i){
            j = (j + 1)/64 - 0.5;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int buff = (int)round(gamma_remove((double) buffer[i* width + j]));
            buff += (int)round(ordered_l[i%8][j%8]*255);
            buff = min(buff,255);
            buff = max(buff,0);
            buff = findNear(buff);
            buffer[i* width + j] = round ((int) (gamma_set((double)buff)));
        }
    }
}

void PNM_Dith::random() {
    srand(time(NULL));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int buff = (int)round(gamma_remove((double) buffer[i* width + j]));
            buff += rand() % 64;
            buff = min(buff,255);
            buff = max(buff,0);
            buff = findNear(buff);
            buffer[i* width + j] = round ((int) (gamma_set((double)buff)));
        }
    }
}

void PNM_Dith::floyd() {
    vector<vector<double >> data_mistakes(height,vector<double>(width,0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int buff = (int)round(gamma_remove((double) buffer[i* width + j]));
            buff += (int)round(data_mistakes[i][j]);
            int buff2 = findNear(buff);
            double mistake = buff - buff2;
            if (i + 1 < height){
                data_mistakes[i+1][j] += mistake*5/16;
                if (j + 1 < width){
                    data_mistakes[i+1][j+1] += mistake/16;
                }
                if (j - 1 >= 0){
                    data_mistakes[i+1][j+1] += mistake*3/16;
                }
            }
            if (j + 1 < width){
                data_mistakes[i][j+1] += mistake*7/16;
            }
            buffer[i* width + j] = round ((int) (gamma_set((double)buff2)));
        }
    }
}

void PNM_Dith::jarvis() {
    vector<vector<double >> data_mistakes(height,vector<double>(width,0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int buff = (int)round(gamma_remove((double) buffer[i* width + j]));
            buff += (int)round(data_mistakes[i][j]);
            int buff2 = findNear(buff);
            double mistake = buff - buff2;
            for (int k = 0; k < 3;k++){
                for (int l = 0; l < 5;l++){
                    if (j + l - 2 < width and j + l - 2 >= 0 and i + k < height){
                        data_mistakes[i + k][j + l - 2] += mistake*JJN[k][l]/48;
                    }
                }
            }
            buffer[i* width + j] = round ((int) (gamma_set((double)buff2)));
        }
    }
}

void PNM_Dith::sierra() {
    vector<vector<double >> data_mistakes(height,vector<double>(width,0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int buff = (int)round(gamma_remove((double) buffer[i* width + j]));
            buff += (int)round(data_mistakes[i][j]);
            int buff2 = findNear(buff);
            double mistake = buff - buff2;
            for (int k = 0; k < 3;k++){
                for (int l = 0; l < 5;l++){
                    if (j + l - 2 < width and j + l - 2 >= 0 and i + k < height){
                        data_mistakes[i + k][j + l - 2] += mistake*Sierra3[k][l]/32;
                    }
                }
            }
            buffer[i* width + j] = round ((int) (gamma_set((double)buff2)));
        }
    }
}

void PNM_Dith::atkinson() {
    vector<vector<double >> data_mistakes(height,vector<double>(width,0));
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int buff = (int)round(gamma_remove((double) buffer[i* width + j]));
            buff += (int)round(data_mistakes[i][j]);
            int buff2 = findNear(buff);
            double mistake = buff - buff2;
            for (int k = 0; k < 3;k++){
                for (int l = 0; l < 5;l++){
                    if (j + l - 2 < width and j + l - 2 >= 0 and i + k < height){
                        data_mistakes[i + k][j + l - 2] += mistake*Atkinson[k][l]/8;
                    }
                }
            }
            buffer[i* width + j] = round ((int) (gamma_set((double)buff2)));
        }
    }
}

void PNM_Dith::halftone() {
    for (auto& i:Halftone){
        for (auto& j:i){
            j = (j+1)/16 - 0.75;
        }
    }
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int buff = (int)round(gamma_remove((double) buffer[i* width + j]));
            buff += (int)round(Halftone[i%4][j%4]*255);
            buff = min(buff,255);
            buff = max(buff,0);
            buff = findNear(buff);
            buffer[i* width + j] = round ((int) (gamma_set((double)buff)));
        }
    }
}

void PNM_Dith::dithering(int argc, char **argv) {
    if (argc != 7) {
        cerr << "Incorrect count of arguments" << endl;
        exit(1);
    }
    string fin = argv[1];
    string fout = argv[2];
    if (stoi(argv[3]) == 1)
        gradient = true;
    else if (stoi(argv[3]) == 0)
        gradient = false;
    else{
        cerr << "Grad must be 1 or 0 and you entered " << stoi(argv[3]) << endl;
        exit(1);
    }
    type_of_dithering = (Dith_type) stoi(argv[4]);
    if (0 > stoi(argv[4]) or stoi(argv[4]) > 7) {
        cerr << "Incorrect type of algorithm, it must be between 0 to 7" << endl;
        exit(1);
    }
    bitRate = stoi(argv[5]);
    if (bitRate < 1 or bitRate > 8){
        cerr << "Incorrect bitrate of result, it must be between 1 to 8" << endl;
        exit(1);
    }
    gamma = stod(argv[6]);
    read(fin);
    if (gradient)
        fill_gradient();
    switch (type_of_dithering){
        case NONE:
            none();
            break;
        case ORDERED_8x8:
            ordered();
            break;
        case RANDOM:
            random();
            break;
        case FLOYD_STEINBERG:
            floyd();
            break;
        case JARVIS_JUDICE_NINKE:
            jarvis();
            break;
        case SIERRA_3:
            sierra();
            break;
        case ATKINSON:
            atkinson();
            break;
        case HALFTONE_4x4:
            halftone();
            break;
    }
    write(fout);
}

void PNM_Dith::fill_gradient() {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            buffer[i * width + j] = (unsigned char) gamma_set(((double) j / width) * 255.0);
        }
    }
}

double PNM_Dith::gamma_set(double value) {
    value = value / 255;
    if (gamma == 0) {
        if (value < 0.0031308)
            return value * 12.92 * 255.;
        else
            return 255. * (1.055*pow(value,1/2.4) - 0.055);
    }
    else {
        return 255. * pow(value, 1 / gamma);
    }
}

double PNM_Dith::gamma_remove(double value) {
    value = value / 255;
    if (gamma == 0.) {
        if (value < 0.04045)
            return 255. * value / 12.92;
        else
            return 255. * pow(((value+ 0.055)/1.055),2.4);
    }
    else {
        return 255. * pow (value, gamma);
    }
}

int PNM_Dith::findNear(int buff) {
    vector<short> data(8,0);
    for (int j = 7; j >= 0; --j) {
        if (j - 8 + bitRate >= 0){
            if (buff > pow(2,j)){
                data[7 - j] = 1;
                buff -= pow(2,j);
            } else{
                data[7 - j] = 0;
            }
            continue;
        }else {
            data[7 - j] = data[(7 - j) % bitRate];
        }
    }
    buff = 0;
    for (int k = 7; k >= 0; --k) {
        buff += data[k]* (int)pow(2,7 - k);
    }
    return buff;
}






