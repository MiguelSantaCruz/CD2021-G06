/*
Autores: João Nunes - A87972 , André Silva - A87958
Data: 01/01/2020
Versão: Final

Ficheiro onde são declaradas as funções usadas.
É aqui também declarada a estrutura de dados usada no programa.
*/

#ifndef __MODULO_B_H__
#define __MODULO_B_H__

#define NUMBER_OF_FREQ 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct node {   //Estrutura de dados com informações de cada simbolo
    int init_symbol;    //Posição inicial do simbolo no ficheiro inicial
    int final_symbol;   //Posição final do simbolo no ficheiro inicial
    int freq;           //Frequencia do simbolo
    char code[256];     //Codigo do simbolo
}node;

void readFreqs (FILE *freq, int indexPointer, char *buffer, long long block_size);

int distArroba (FILE *freq);

void toStruct (struct node nodes[], char *buffer, int *freq_Array, int pos_freqs, int block_size);

void arraySort (int *freq_Array, int array_size);

int calcular_melhor_divisao (int *freqArray, int i, int j);

void calcular_codigos_SF (int *freqArray, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col);

int soma (int *freqArray, int i, int j);

void add_bit_to_code (char c, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col);

char *add_bit_to_code_AUX(int start, int len);

void itoa(int n, char s[]);

void reverse(char s[]);

void moveToBuffer(struct node nodes[], char *exitBuffer, int posmove);

#endif