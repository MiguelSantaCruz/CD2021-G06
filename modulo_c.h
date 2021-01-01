#ifndef __MODULO_C_H__
#define __MODULO_C_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include "main.h"
//#include "modulo_a.h"
//#include "modulo_b.h"
//#include "modulo_d.h"

#define READ_SIZE 4096
#define BUF_SIZE 17
#define CODE_SIZE 24
#define NUMBER_OF_SYMBOLS 256

//Struct que guarda informação acerca do indice de leitura dos buffer do ficheiro .cod e do ficheiro a comprimir
typedef struct positionIndicator{
    int codIndex;
    int fileIndex;
} *positionIndicator;

typedef struct fileData{
    int verbose;
    int firstTime;
    int rOrN;
    int blockNumber;
    int blockSize;
    int blockIndex;
    int endOfFile;
    int fileSize;
    int compressedFileSize;
    FILE *cod;
    FILE *file;
    FILE *shaf;
    char shafFilename[50];
} *fileData;

//Função que lê o ficheiro .cod e cria uma matriz com os códigos dos simbolos
void readCodFile(char* ,positionIndicator* ,fileData* );

//Função que lê do ficheiro o bloco e escreve a matriz dos codigos
void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],fileData* ,positionIndicator* ,char* );

//Função que escreve um bloco traduzido para o ficheiro shaf
void writeBlockToShaf(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],fileData*);
//Função que mostra a matriz no terminal
void printMatrix(char matrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1]);

void readFileandBlocksInfo(positionIndicator* ,fileData* ,char* );

int readToBufferIfNeeded(fileData* ,positionIndicator* ,char* );

void initializeStructs(fileData* ,positionIndicator* );

void commandLine(int ,char**,fileData*);

void openFiles(char**,fileData*);

void printError(int);

void printRunDetails(fileData* ,struct timespec* ,struct timespec* );


#endif