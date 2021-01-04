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

//Tamanho do buffer de leitura do ficheiro .cod
#define READ_SIZE 4097
//Tamanho do buffer temporario para guardar informações diversas ,i.e, tamanho do bloco etc.
#define BUF_SIZE 30
//Tamanho máxio dos códigos Shanon Fano dos símbolos
#define CODE_SIZE 30
//Número de símbolos 
#define NUMBER_OF_SYMBOLS 256

//Struct que guarda informação acerca do indice de leitura dos buffer do ficheiro .cod e do ficheiro a comprimir
typedef struct positionIndicator{
    //Indice de leitura no ficheiro .cod
    int codIndex;
} *positionIndicator;

//Struct que guarda informações diversas sobre o ficheiro
typedef struct fileData{
    //Indica se foi usada a opção -v
    int verbose;
    //Indica se estamos a ler o ficheiro .cod pela primeira vez
    int firstTime;
    //Bool para indicar se o ficheiro é RLE ou não
    int rle;
    //Número de blocos 
    int blockNumber;
    //Tamanho de cada bloco
    int blockSize;
    //Indíce do bloco atual
    int blockIndex;
    //Bool para final do ficheiro
    int endOfFile;
    //Tamanho do ficheiro
    int fileSize;
    //Tamanho do ficheiro comprimido
    int compressedFileSize;
    //Apontadores para os ficheiros .cod, original e .shaf respetivamente
    FILE *cod;
    FILE *file;
    FILE *shaf;
    //Nome do fichereiro de saída
    char shafFilename[50];
} *fileData;

//Main deste módulo
int mainModuloC(int argc,char* argv[]);
//Função que lê o ficheiro .cod e cria uma matriz com os códigos dos simbolos
void readCodFile(char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1] ,char* ,positionIndicator* ,fileData* );
//Função que lê do ficheiro o bloco e escreve a matriz dos codigos
void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],fileData* ,positionIndicator* ,char* );
//Função que escreve um bloco traduzido para o ficheiro shaf
void writeBlockToShaf(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],fileData*);
//Função que mostra a matriz no terminal
void printMatrix(char matrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1]);
//Ler informação acerca do número de blocos e respetivos tamanhos
void readFileandBlocksInfo(positionIndicator* ,fileData* ,char* );
//Ler mais bytes do ficheiro se for necessário
int readToBufferIfNeeded(fileData* ,positionIndicator* ,char* );
//inicializar os structs que contem informações sobre os ficheiros
void initializeStructs(fileData* ,positionIndicator* );
//Função que lê as opções instroduzidas no terminal
void commandLine(int ,char**,fileData*);
//Função auxiliar que inicializa os ficheiros
void openFiles(char**,fileData*);
//Mostrar no terminal o erro
void printError(int);
//Mostrar detalhes de execução
void printRunDetails(fileData* ,struct timespec* ,struct timespec* );

#endif
