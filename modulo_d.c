#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modulo_d.h"

int main (int argc, char argv[]){

	if (argc /= 4){
		printf("O número de argumentos não é correto\nSintaxe: ./shafa (ficheiro a ser comprimido) -m d\n");
        exit(1);
    }

    //Lê ficheiros .shaf e .cod
    if (strcomp(argv[2], "-m") && strcomp(argv[3], "d")){
    	//TODO
    }

    //Lê ficheiros .rle e .freq
	if (strcomp(argv[2], "-d") && strcomp(argv[3], "s")){
    	//TODO Decompressão SF
    }

    //Lê ficheiros .rle e .freq
    if (strcomp(argv[2], "-d") && strcomp(argv[3], "r")){
    	//TODO Decompressão RLE
    }    
}

void readShaf(FILE *shaf, FILE cod){
	//TODO (pode precisar de mais argumentos)
}


void readRLE(FILE *rle, FILE *freq){
	//TODO (pode precisar de mais argumentos)
}