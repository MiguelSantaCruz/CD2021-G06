#ifndef __MODULO_B_H__
#define __MODULO_C_H__
#define NUMBER_OF_FREQ 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "modulo_a.h"
#include "modulo_c.h"
#include "modulo_d.h"

typedef struct node {
    int init_symbol; //109
    int final_symbol;
    int freq; // 768
    char code[256]; // 10
}node;

void readFreqs (FILE *freq, int indexPointer, char *buffer, long long block_size);

int distArroba (FILE *freq);

void toStruct (struct node nodes[], char *buffer, int *freq_Array, int pos_freqs);

void minSort (int *freq_Array, int array_size);

int calcular_melhor_divisao (int *freqArray, int i, int j);

void calcular_codigos_SF (int *freqArray, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col);

int soma (int *freqArray, int i, int j);

void add_bit_to_code (char c, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col);

char *add_bit_to_code_AUX(int start, int len);

void itoa(int n, char s[]);

void reverse(char s[]);

void moveToBuffer(struct node nodes[], char *exitBuffer, int posmove);

#endif