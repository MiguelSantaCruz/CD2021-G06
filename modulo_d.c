#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modulo_d.h"
#include <math.h>
#include <ctype.h>

#define READ_SIZE 4081
#define BUF_SIZE 17
#define CODE_SIZE 16
#define NUMBER_OF_SYMBOLS 256

int main (int argc, char argv[]){

    /*    
	//Lê ficheiros .shaf e .cod
    if (strcmp(argv[2], "-m") && strcmp(argv[3], "d")){
        printf("entrou");
        char shafFilename[strlen(argv[1])];
        strcpy(shafFilename,argv[1]);     [shafa, ...]
        char codFilename[strlen(shafFilename)+9];
        strncpy(codFilename,shafFilename,strlen(shafFilename)-5);
        strcat(codFilename,".cod");

        FILE *shaf = fopen(shafFilename,"r");
        FILE *cod = fopen(codFilename,"r");
        printf("teste\n");

        if(shaf == NULL) {
            printf ("Erro: Ficheiro original inexistente!\n");
            exit(2);
        }
        if(cod == NULL){
            printf ("Erro : Ficheiro .cod inexistente!\n");
            exit(3);
        }
    }

    //Gera original de shaf
    if (strcmp(argv[3], "-d") && strcmp(argv[4], "r")){
    
    }

    //Gera Rle de shaf
    if (strcmp(argv[2], "-d") && strcmp(argv[3], "s")){
        //TODO Decompressão RLE
    }

    if (argc > 4){
        printf("O número de argumentos não é correto\nSintaxe: ./shafa (ficheiro a ser comprimido) -m d\n");
        exit(1);
    }
    */

    //mudei para w
    FILE *file = fopen("aaa1.txt", "w");
    FILE *rle = fopen("aaa.txt.rle", "r");
    FILE *shaf = fopen("aaa.txt.shaf", "r");
    FILE *cod = fopen ("aaa.txt.cod", "r");

    //readRLE(rle, NULL, file, 3000);
    readShaf(shaf,cod,file, 3000);
    //readCod(cod, 3000);
    return 0;    
}

void readShaf(FILE *shaf, FILE *cod, FILE *file, long int shafsize){
	//TODO (pode precisar de mais argumentos)
    unsigned char bufferShaf[shafsize];
    unsigned char nextbuffer[shafsize];
    int i;
    int j;
    int contar = 0;
    unsigned int m;
    char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1];

    //lê shaf
    fread(bufferShaf,1,shafsize,shaf);
    printf("%s\n", bufferShaf);
    //printf("%lld\n", buffer[7]);
    
    //passa à frente os '@'
    for (i = 0; contar < 3; i++)
        if (bufferShaf[i] == '@') contar++;
    for (j = i; bufferShaf[j] != '@'; j++);
    
    //mudo o apontador para o inicio do conteudo do bloco
    fseek(shaf, i, SEEK_SET);
    fread(nextbuffer,1, j - i, shaf);

    char aux[10000];
    aux[0] = '\0';
    j = 0;
    int c = 0;


    //mete num array bit a bit
    for(i = 0; i < strlen(nextbuffer); i++){
        char aux1[64];
        m = decimalToBinary(charToInt(nextbuffer[i]));    
        c+=4;
        sprintf(aux1, "%d", m);
        strcat(aux, aux1);
        aux1[0] = '\0';
    }
    
    //Elimina o lixo 
    for(i = 0; i ;i++)
        if(aux[i] != 1 && aux[i] != 0){
            aux[i] = '\0';
            break;
        }

    // Debugging    
    for (i = 0; aux[i] != '\0'; i++)
        printf("%c", aux[i]);
    printf("\n");

}

int charToInt(char *c){
    int i;
    i = (int)(c);
    return i;
}
/*
int countBits(unsigned int number) 
{       
      // log function in base 2  
      // take only integer part 
      return (int)((log(number)+1)/(log(2)));
}
*/
long decimalToBinary(int decimalnum)
{
    long binarynum = 0;
    int rem, temp = 1;

    while (decimalnum!=0)
    {
        rem = decimalnum%2;
        decimalnum = decimalnum / 2;
        binarynum = binarynum + rem*temp;
        temp = temp * 10;
    }
    return binarynum;
}

void readCod(FILE *cod,long int shafsize){
    //TODO (pode precisar de mais argumentos)
    unsigned char bufferCod[shafsize];
    unsigned char nextbuffer[shafsize];
    int i;
    int j;
    int contar = 0;
    int nd;
    char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1];
    int conta;
    fread(bufferCod,1,shafsize,cod);
    //printf("%s\n", bufferCod);
    //printf("%lld\n", buffer[7])

    for (i = 0; contar < 4; i++)
        if (bufferCod[i] == '@') contar++;
    for (j = i; bufferCod[j] != '@'; j++);
    fseek(cod, i, SEEK_SET);
    fread(nextbuffer,1, j - i, cod);

    //printf("\n%s\n", nextbuffer);

    int str[256];
    int pos = 0;
    for(i = 0; i<256; i++) str[i] = -1;
    for (i = 0; i < strlen(nextbuffer); i++, pos++){
        int aux[16];
        j = 0;
        while(nextbuffer[i] != ';') {
            
            aux[j++] = nextbuffer[i]-48;
                    
            i++;
        }
        aux[j] = NULL;
        if (j > 0){

            str[pos] = array_to_num(aux, j);

        }    
        //sprintf(aux, "%d", )  
    }
    for (i = 0; i < 256; i++)
        printf("%d ",str[i]);
    //printMatrix(str);
    printf("\n");
    printf("97 - %d, 98- %d, 99 - %d, 100 - %d\n", str[97],str[98],str[99],str[100]);

    //printf("%u\n", buffer);
    /*
    char nb[3];
    for (int j = 0; j < 3; j++)
        nb[j] = buffer[i++];
    printf("%s\n", nb);
    */
}

int array_to_num(int arr[],int n){
    char str[6][3];
    int i;
    char number[13] = {'\n'};

    for(i=0;i<n;i++) sprintf(str[i],"%d",arr[i]);
    for(i=0;i<n;i++)strcat(number,str[i]);

    i = atoi(number);
    return i;
} 

void readRLE(FILE *rle, FILE *freq, FILE *file, int rlesize){
    //FIX
    int i;
    int j;
    
    char newbuffer[rlesize];

    fread(newbuffer, sizeof(char), rlesize, rle);

/*
    for (i = 0; i < 1494; i++){
        if (newbuffer[i] == NULL) 
            printf("0");
        else if (isalpha(newbuffer[i])) printf("%c", newbuffer[i]);
        else printf("%d", newbuffer[i]);
    }
*/

    for (i = 0; newbuffer[i] != EOF; i++){
        if (newbuffer[i] == 0){
            for (j = 0; j < newbuffer[i + 2]; j++){
                if (!(newbuffer[i+1] > 96 && newbuffer[i] < 101)) printf("%d ", i); 
                fputc(newbuffer[i+1], file); 
            }
            i+=2;         
        }
        else fputc(newbuffer[i], file);
    }
}

