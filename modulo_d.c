#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modulo_d.h"
#include <math.h>

#define READ_SIZE 4081
#define BUF_SIZE 17
#define CODE_SIZE 16
#define NUMBER_OF_SYMBOLS 256

int main (int argc, char argv[]){

    /*    
	//Lê ficheiros .shaf e .cod
    if (strcmp(argv[2], "-m") && strcmp(argv[3], "d")){
        printf("entrou");
        char shafFilename[strlen(argv[1])];
        strcpy(shafFilename,argv[1]);
        char codFilename[strlen(shafFilename)+9];
        strncpy(codFilename,shafFilename,strlen(shafFilename)-5);
        strcat(codFilename,".cod");

        FILE *shaf = fopen(shafFilename,"r");
        FILE *cod = fopen(codFilename,"r");
        printf("teste\n");

        if(shaf == NULL) {
            printf ("Erro: Ficheiro original inexistente!\n");
            exit(2);
        }
        if(cod == NULL){
            printf ("Erro : Ficheiro .cod inexistente!\n");
            exit(3);
        }
    }

    //Gera original de shaf
    if (strcmp(argv[3], "-d") && strcmp(argv[4], "r")){
    
    }

    //Gera Rle de shaf
    if (strcmp(argv[2], "-d") && strcmp(argv[3], "s")){
        //TODO Decompressão RLE
    }

    if (argc > 4){
        printf("O número de argumentos não é correto\nSintaxe: ./shafa (ficheiro a ser comprimido) -m d\n");
        exit(1);
    }
    */

    //mudei para w
    FILE *file = fopen("aaa1.txt", "w");
    FILE *rle = fopen("aaa.txt.rle", "r");
    

    readRLE(rle, NULL, file, 3000);
    return 0;    
}

void readShaf(FILE *shaf, FILE *cod){
	//TODO (pode precisar de mais argumentos)
}

void writeShafToTxt(FILE *shaf, FILE *file){
    
}

void readRLE(FILE *rle, FILE *freq, FILE *file, long int rlesize){
    //FIX
    int i;
    long long int j;
    
    char newbuffer[rlesize];
    char* str;

    fread(newbuffer, sizeof(char), rlesize, rle);

    for (i = 0; newbuffer[i] != EOF; i++){
        if (newbuffer[i] == NULL){
            for (j = 0; j < newbuffer[i + 2]; j++)
                fputc(newbuffer[i+1], file); 
            i+=2;         
        }
        else fputc(newbuffer[i], file);
    }
    fputc('\n', file);
}
