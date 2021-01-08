#ifndef __MODULO_D_H__
#define __MODULO_D_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//Tamanho máximo dos códigos SF
#define CODE_SIZE 16

//Número de simbolos ASCII
#define NUMBER_OF_SYMBOLS 256

//Main do módulo
int mainModuloD(int argc, char* argv[]);

//Lê .rle e .freq e escreve para um ficheiro .txt
void readRLE(FILE *rle, FILE *freq, FILE *file, int rlesize);

//Lê cod
void readCod(FILE *cod, char* binShaf, FILE* file,int* firsttime, int* indexpointerCod);

//Devolve a posição de str onde str[simbolo][codigo] == codigo SF
int findSF(char str[NUMBER_OF_SYMBOLS][CODE_SIZE], char aux[CODE_SIZE]);

//Lê shaf
void readShaf(FILE *shaf, FILE *cod, FILE *file,int* firsttime, int* indexpointerCod, int* indexpointerShaf);

//Conta o tamanho do RLE lendo o tamanho de cada bloco do freq
int contaFilesizeRle(FILE *freq);

//Converte um número decimal para binário
int  decimalToBinary(int decimalnum);

//Converte Char para Int
int charToInt(unsigned char *c);

//Conta o número de digitos de um número decimal
int contaDigitos (int num);

#endif