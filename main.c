#include <stdio.h>
//#include "modulo_a.h"
//#include "modulo_b.h"
#include "modulo_c.h"
//#include "modulo_d.h"
//#include "fsize.h"

int main(int argc,char* argv[]){
    char c;
    if(strcmp(argv[2],"-m")) printError(-6);
    sscanf(argv[3],"%c",&c);
    switch (c)
    {
    case 'f':
        //mainModuloA(argc,argv);
        break;
    case 't':
       //mainModuloB(argc,argv);
        break;
    case 'c':
        mainModuloC(argc,argv);
        break;
    case 'd':
        //mainModuloD(argc,argv);
        break;
    
    default:
    printError(-6);
        break;
    }
    return 0;
}