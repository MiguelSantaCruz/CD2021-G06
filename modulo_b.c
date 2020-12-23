#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fsize.h"

int main (int agrc, char* argv[]){

    if(agrc<2){
        printf ("Número de argumentos incorreto.\nUsar: ./shafa exemplo.txt.freq -m t");
        exit(1);
    }

    char fileName[strlen(argv[1])+1];
    strcpy(fileName,argv[1]);
    
    FILE * ficheiroFreq = fopen (fileName, "r");

    if (ficheiroFreq == NULL){
        printf ("ERRO: Ficheiro não existe!\n");
    }
    
    unsigned long long total;
    long long n_blocks;
    unsigned long block_size;
    long size_of_last_block;

    int index = 0;
    char c; 
    char a = '@';
    char c_block_size[100];
    int arrobas = 0;
    int rOrN = 0; // N = 1 R = 0

    while ((c = fgetc(ficheiroFreq)) != EOF){
        index++;
        if (c == 'N') rOrN = 1;

        if (a == c) arrobas++;

        else if (arrobas == 3){
            strcat (c_block_size, &c);
        }

        if (arrobas == 4){
            break;
        }
    }

    rewind(ficheiroFreq);

    block_size = atoi(c_block_size);
    n_blocks = fsize(ficheiroFreq, NULL, &block_size, &size_of_last_block);
    total = (n_blocks-1) * block_size + size_of_last_block;

    //Função que le
    int i = 0;
    int *indexpointer = &index+1;
    char *entry_buffer[total+1];

    while (i<n_blocks-1){
        readFreqs(ficheiroFreq, *indexpointer , *entry_buffer, block_size);
        i++;
        index += block_size;
    }
    readFreqs (ficheiroFreq, *indexpointer, *entry_buffer, size_of_last_block);
    printf ("%s", entry_buffer);

    fclose (ficheiroFreq);

    //FUnção que shano
    //Função que escreve 

    printf ("André Silva - A87958 , João Nunes - A87972\n");
    printf ("MIEI/CD, 2020\n");
    printf ("Módulo B: Cálculo das tabelas de codificação\n");
    printf ("Ficheiro: %c \n", fileName);
    printf ("Número de blocos: %llu\n", n_blocks);
    printf ("Tamanho dos blocos analisados no ficheiro de frequências: %ld blocos de %lu bytes + 1 bloco de %lu bytes\n", n_blocks-1, block_size, size_of_last_block);
    printf ("Tamanho total do ficheiro: %llu\n", total);
    printf ("Tempo de execução: \n");
    printf ("Ficheiro gerado: \n");

}

void readFreqs (FILE *freq, int *indexPointer, char *buffer, long long block_size){
    printf("Deu certo");
    int i = *indexPointer;

    fread(buffer, sizeof(char), block_size, freq);
}




