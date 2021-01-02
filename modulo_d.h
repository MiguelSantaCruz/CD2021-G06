#ifndef __MODULO_D_H__
#define __MODULO_D_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
//#include "main.h"
//#include "modulo_a.h"
//#include "modulo_b.h"
//#include "modulo_c.h"

//Lê .rle e cria .txt
void readRLE(FILE *rle, FILE *freq, FILE *file, int rlesize);

//Lê cod
void readCod(FILE *cod,long int shafsize);

//Array de inteiros para um inteiro só
int array_to_num(int arr[],int n);

//Lê shaf
void readShaf(FILE *shaf, FILE *cod, FILE *file, long int shafsize);

long decimalToBinary(int decimalnum);

int charToInt(char *c);

#endif