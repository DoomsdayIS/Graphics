#include <iostream>
#include "picture.h"

int main(int argc, char* argv[]){
    Picture NewPicture;
    try {
        NewPicture.read(argc,argv);
    }catch (std::exception& exp){
        std::cerr << exp.what() << std::endl;
        return 1;
    }
    return 0;
}