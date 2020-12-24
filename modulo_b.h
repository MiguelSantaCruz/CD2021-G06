#ifndef __MODULO_B_H__
#define __MODULO_C_H__

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
    int code; // 10
}node;

void readFreqs (FILE *freq, int *indexPointer, char *buffer, long long block_size);

int distArroba (FILE *freq, int size, int x);

void toStruct (struct node nodes[256], char *buffer, long long n_blocks, char *freq_Array);

#endif