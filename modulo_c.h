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
#define CODE_SIZE 17
#define NUMBER_OF_SYMBOLS 255

//Função que lê o ficheiro original/RLE e codifica o simbolos
void readFile(FILE *file,FILE *shaf,char *codes);

//Função que escreve o ficheiro .shaf
void writeFile(FILE *shaf,char *c);

//Função que lê o ficheiro .cod e cria uma matriz com os códigos dos simbolos
char* readCod(FILE *cod,char *p);

//Função que completa a sequencia com zeros
void completeWithZeros(char *code,int codeBit);

#endif //__MODULO_C_H__