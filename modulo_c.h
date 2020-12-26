#ifndef __MODULO_C_H__
#define __MODULO_C_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "modulo_a.h"
#include "modulo_b.h"
#include "modulo_d.h"

#define READ_SIZE 4081
#define BUF_SIZE 17
#define CODE_SIZE 16
#define NUMBER_OF_SYMBOLS 256

//Função que lê o ficheiro .cod e cria uma matriz com os códigos dos simbolos
void readCod(FILE *cod,FILE *file,FILE* shaf,int *indexPointer,int *endFilePointer,int firstTime,int blockIndex);

//Função que lê do ficheiro o bloco e escreve a matriz dos codigos
void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],int *index, int symbol,char *code,char* string);

//Função que escreve um bloco traduzido para o ficheiro shaf
void writeBlockToShaf (char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],int blockSize,int blockNumber,int firstTime,FILE *cod,FILE *file,FILE *shaf);

#endif //__MODULO_C_H__