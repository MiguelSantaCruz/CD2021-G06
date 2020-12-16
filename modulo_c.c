#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modulo_c.h"


#define READ_SIZE 4081
#define BUF_SIZE 17
#define CODE_SIZE 16
#define NUMBER_OF_SYMBOLS 256

//Função que lê o ficheiro .cod e cria uma matriz com os códigos dos simbolos
void readCod(FILE *cod,FILE *file,FILE* shaf,int *indexPointer,int *endFilePointer,int firstTime);

//Função que lê do ficheiro o bloco e escreve a matriz dos codigos
void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int *index, int symbol,char *code,char* string);

void printMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE]);

void writeBlockToShaf (char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int blockSize,FILE *cod,FILE *file,FILE *shaf);

int main(){
    int index = 0;
    int endFile = 0;
    int *endFilePointer = &endFile;
    int *indexPointer = &index;
    int initialTime = time(NULL);
    int finalTime;
    FILE *file = fopen("aaa.txt","r");
    FILE *cod = fopen("aaa.txt.rle.cod","r");
    if(file == NULL) printf ("Ficheiro original inexistente!\n");
    if(cod == NULL) printf ("Ficheiro .cod inexistente!\n");
    FILE *shaf = fopen("aaa.txt.shaf","w");
    printf("[Miguel Santa Cruz e Marisa Soares, MIEI, 2020]\n");
    printf("[Modulo C - Codificação do ficheiro original/RLE]\n");
    int firstTime = 1;
    for (int i = 0; *endFilePointer != 1; i++)
    {
        readCod(cod,file,shaf,indexPointer,endFilePointer,firstTime);
        fseek(cod,index,SEEK_SET);
        firstTime = 0;
    }
    finalTime = time(NULL);
    printf("Tempo total : %d s (devia estar em milissegundos)\n Falta tb a taxa de compressão de cada bloco e taxa global de compressao\n",finalTime-initialTime);
    return 0;
}

void readCod(FILE *cod,FILE *file,FILE* shaf,int *indexPointer,int *endFilePointer,int firstTime){
    
    //ponteiro para o indíce em que já foi lida a string lida do ficheiro
    int index = *indexPointer;
    
    //String que contem 8 bytes lidos do ficheiro .cod
    char string[READ_SIZE];

    //Buffer que contem informaçao aleatória
    char buffer[BUF_SIZE];

    int blockNumber,blockSize;
    
    //Inteiro que guarda a R (0) ou N(1)
    int rOrN = 0;

    //Código do símbolo lido do ficheiro
    char code[CODE_SIZE+1];

    //Matriz dos códigos de cada símbolo
    char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE]; 
    
    //Obter os primeiros READ_SIZE bytes do ficheiro
    fgets(string,READ_SIZE,cod);
    index++;
    
    int constant;
    if(firstTime){
        //Ler a informação ficheiro original/RLE
        if(string[index]=='N') rOrN = 1; 

        //Debugging ---------------------------------------
        //printf("R(0) ou N(1): %d\n",rOrN);
        //-------------------------------------------------

        //Ler o número de blocos e guardar no array blockNumber
        constant = index+2;
        for (int i = constant; string[i]!='@'; i++){
            buffer[i-constant] = string[i];
            index = i;
        }
        blockNumber = atoi(buffer);
        printf("[Número de blocos: %d]\n",blockNumber);

        //Debugging ---------------------------------------    
        //printf("Numero de blocos: %d\n",blockNumber);
        //-------------------------------------------------
        constant = index+2;
    }
    if(!firstTime) constant = ++index;
    //Ler o tamanho do 1º bloco e guardar no array blockSize
    
    for (int i = constant; string[i] != '@'; i++){
        buffer[i-constant]=string[i];
        index = i;
    }
    index++;
    blockSize = atoi(buffer);
    printf("[Tamanho do bloco: %d]\n",blockSize);
    //Debugging ----------------------------------------
    //printf("Tamanho do bloco: %d\n",blockSize);
    //--------------------------------------------------
    if (blockSize == 0) {
        *endFilePointer = 1;
        return;
    }
    //Indice da sequencia de bits do simbolo
    int codeBit = 0;

    //Indice do símbolo na matriz
    int symbol = 0;
    *indexPointer = index;
    readAndWriteToMatrix(codesMatrix,indexPointer,symbol,code,string);
    writeBlockToShaf(codesMatrix,blockSize,cod,file,shaf);
    //printMatrix(codesMatrix);
    return;
}

void writeBlockToShaf (char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int blockSize,FILE *cod,FILE *file,FILE *shaf){
    char buffer[blockSize+1];
    char binaryCodes[CODE_SIZE*blockSize+1];
    int count = 0;
    int indexCode = 0;
    fread(buffer,sizeof(char),blockSize,file);
    for (int i = 0 ; buffer[i]!='\0'; i++) {
        for (int j = 0; j<CODE_SIZE; j++)
        {
            binaryCodes[indexCode]= codesMatrix[buffer[i]][j];
            indexCode++;
        }
        count++;  
    }
    fwrite(binaryCodes,sizeof(char),count,shaf);
    return;
}

void printMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE]){
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++)
    {
        printf("%d: ",i);
        for (int j = 0; codesMatrix[i][j]!='\0'; j++)
        {
            printf("%c ",codesMatrix[i][j]);
        }
        printf("\n");
    }
}

void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int *indexPointer, int symbol,char *code,char* string){
    int index = *indexPointer;
    int constant = index+2;
    int codeBit;
    for (int i = constant; string[i]!='@'; i++){
        codeBit = 0;
        for (int j = 0; string[i]!=';'; j++){
            code[j]=string[i];   
            codeBit++;
            i++;
        }
        if (codeBit != 0) {
            code[codeBit]='\0';   
            for (int k = 0; code[k]!='\0'; k++) codesMatrix[symbol][k]=code[k];
        } else codesMatrix[symbol][0]='\0';
        symbol++;
        index = i;
    }
    *indexPointer = index;
    return;
}


//Algoritmo do professor
/* binary_coding(symbols[],number_of_symbols,codes[],index[],next[]){ 
    offset=0, ind_in=0, ind_out=0;
    while (ind_in<number_of_symbols){ 
        symbol=symbols[ind_in]+offset;
        n_bytes_in_code=index[symbol];
        code=codes[symbol];
        ind_code=0;
    while (ind_code≤n_bytes_in_code){
        coded_sequence[ind_out]=OR(coded_sequence[ind_out],code[ind_code])
        if (ind_code<n_bytes_in_code then){
            ind_out=ind_out+1;
            ind_code=ind_code+1;
        }
    }
    offset=next[symbol];
    ind_in=ind_in+1;
    }
binary_coding=coded_sequence;
} */
