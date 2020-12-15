#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulo_c.h"

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

int main(){
    char *p ;
    FILE *file = fopen("aaa.txt","r");
    FILE *cod = fopen("aaa.txt.rle.cod","r");
    if(file == NULL) printf ("Ficheiro original inexistente!\n");
    if(cod == NULL) printf ("Ficheiro .cod inexistente!\n");
    FILE *shaf = fopen("aaa.txt.shaf","w");
    char *codes = readCod(cod,p);
    readFile(file,shaf,codes);
    return 0;
}

char * readCod(FILE *cod,char *p){
    
    //indíce em que já foi lida a string lida do ficheiro
    int index=0;

    //String que contem 8 bytes lidos do ficheiro .cod
    char string[READ_SIZE];

    //Número de blocos lido do ficheiro
    char blockNumber[BUF_SIZE];

    //Tamanho do bloco lido do ficheiro
    char blockSize[BUF_SIZE];

    //Código do símbolo lido do ficheiro
    char code[CODE_SIZE];

    //Matriz dos códigos de cada símbolo
    char codesMatrix[CODE_SIZE][NUMBER_OF_SYMBOLS-1];
    p = codesMatrix[0];

    //Inteiro que guarda a R (0) ou N(1)
    int rOrN = 0;
    
    
    //Obter os primeiros 8 bytes do ficheiro
    fgets(string,READ_SIZE,cod);

    //Ler a informação ficheiro original/RLE
    if(string[1]=='N') rOrN = 1; 

    //Debugging ---------------------------------------
    printf("R(0) ou N(1): %d\n",rOrN);
    //-------------------------------------------------

    //Ler o número de blocos e guardar no array blockNumber
    for (int i = 3; string[i]!='@'; i++){
        blockNumber[i-3] = string[i];
        index = i;
    }

    //Debugging ---------------------------------------    
    printf("Numero de blocos: %d\n",atoi(blockNumber));
    //-------------------------------------------------

    //Ler o tamanho do 1º bloco e guardar no array blockSize
    int constant = index+2;
    for (int i = constant; string[i] != '@'; i++){
        blockSize[i-constant]=string[i];
        index = i;
    }

    //Debugging ----------------------------------------
    printf("Tamanho do 1º bloco: %d\n",atoi(blockSize));
    //--------------------------------------------------

    //Indice da sequencia de bits do simbolo
    int codeBit = 0;

    //Indice do símbolo na matriz
    int symbol = 0;
    
    //Ler a sequncia de bits correspondente ao simbolo '0'
    constant = index+2;
    for (int i = constant; string[i]!=';'; i++){
        code[i-constant] = string[i];
        codeBit++;
        index = i;
    }

    completeWithZeros(code,codeBit);

    //Escrever na matrix o código correspondente ao símbolo 0
    for (int i = 0; i < CODE_SIZE-1; i++)  codesMatrix[i][0]=code[i];
    
    //Ler todas as sequencias de bits correspondentes ao 256 simbolos
    constant = index+2;
    symbol = 1;
    for (int i = constant; string[i]!='@'; i++){
        codeBit = 0;
        for (int j = 0; string[i]!=';'; j++){
            code[j]=string[i];   
            codeBit++;
            i++;
        }
        if (codeBit != 0) completeWithZeros(code,codeBit);
        for (int k = 0; k < CODE_SIZE-1; k++) codesMatrix[k][symbol]=code[k];
        symbol++;
    }

    //Debugging----------------------------------------------------
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++)
    {
        printf("%d: ",i);
        for (int j = 0; j < CODE_SIZE-1; j++)
        {
            if (j==8) printf(". ");
            printf("%c ",codesMatrix[j][i]);
        }
        printf("\n");
    }
    //--------------------------------------------------------------
    
    //Retorna a o apontador para a matriz dos códigos
    return p;
}

void completeWithZeros(char *code,int codeBit){
    for (codeBit; codeBit < CODE_SIZE-1; codeBit++) code[codeBit]='0';
    code[CODE_SIZE-1]='\0';
    return;
}

void readFile(FILE *file,FILE *shaf,char *codes){
    return;
}

void writeFile(FILE *shaf,char *s){
    fprintf(shaf,"%s",s);
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
