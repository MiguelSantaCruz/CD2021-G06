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
    FILE *file1 = fopen("aaa2.txt", "w");
    FILE *rle = fopen("aaa.txt.rle", "r");
    FILE *shaf = fopen("aaa.txt.shaf", "r");
    FILE *cod = fopen ("aaa.txt.cod", "r");

    readRLE(rle, NULL, file, 1422);
    //readShaf(shaf,cod,file1, 3000);
    //readCod(cod, 3000, NULL);
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
    int blockSize;
    char buffer[15];

    //lê shaf
    fread(bufferShaf,1,shafsize,shaf);
    //printf("%s\n", bufferShaf);
    //printf("%lld\n", buffer[7]);
    
    //passa à frente os '@'
    for (i = 0; contar < 2; i++)
        if (bufferShaf[i] == '@'){ 
            contar++;
            if (contar == 2) {
                for (j = 0, i+=1; bufferShaf[i] != '@'; j++)
                    buffer[j] = bufferShaf[i++];
                blockSize = atoi(buffer);
            }        
        }    
    for (j = i; bufferShaf[j] != '@'; j++);
    
    //mudo o apontador para o inicio do conteudo do bloco
    fseek(shaf, i, SEEK_SET);
    fread(nextbuffer,1, blockSize, shaf);

    char aux[blockSize];
    aux[0] = '\0';
    j = 0;
    int c = 0;

    
    for (i = 0; nextbuffer[i] != '\0'; i++)
        printf("%d ", nextbuffer[i]);
    

    //mete num array bit a bit
    for(i = 0; nextbuffer[i]!= '\0'; i++){
        char aux1[64];
        m = decimalToBinary(charToInt((unsigned)nextbuffer[i]));    
        c+=4;
        sprintf(aux1, "%d", m);
        strcat(aux, aux1);
        aux1[0] = '\0';
    }
    
    
    
    printf(".shaf\n");   
    //Debugging    
    for (i = 0; aux[i] != '\0'; i++)
        printf("%c", aux[i]);
    printf("\n");
    
    

    //findSF(cd, 10, 256);
    readCod(cod, blockSize, aux, file);
}

int findSF(int *str, int num, int blockSize){      
    int i;
    for (i = 0; i < 256; i++){
        if (str[i] == num) return i;                                                         
    }
    return -2;    
}


int charToInt(unsigned char *c){
    int i;
    i = (int)(c);
    return i;
}


int  decimalToBinary(int decimalnum)
{
    int binarynum = 0;
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
                                            //shaf em binario
void readCod(FILE *cod,long int blockSize, char* newaux, FILE* file){
    //TODO (pode precisar de mais argumentos)
    unsigned char bufferCod[blockSize];
    unsigned char nextbuffer[blockSize];
    int i;
    int j;
    int contar = 0;
    int nd;
    char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1];
    int conta;
    fread(bufferCod,1,blockSize,cod);
    //printf("%s\n", bufferCod);
    //printf("%lld\n", buffer[7])

    for (i = 0; contar < 4; i++)
        if (bufferCod[i] == '@') contar++;
    for (j = i; bufferCod[j] != '@'; j++);
    fseek(cod, i, SEEK_SET);
    fread(nextbuffer,1, j - i, cod);

    //printf("\n%s\n", nextbuffer);                                                                findSF()

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
        aux[j] = '\0';
        if (j > 0){

            str[pos] = array_to_num(aux, j);

        }

            
        //sprintf(aux, "%d", )  
    }
    /*for (i = 0; i < 256; i++)
        printf("%d ",str[i]);
    //printMatrix(str);
    //printf("\n");
    //printf("97 - %d, 98- %d, 99 - %d, 100 - %d\n", str[97],str[98],str[99],str[100]);    [1,0,0,0]
    */ 
    char aux[16];
    for (i = 0; i < 16; i++)   
        aux[i] = '\0';
    aux[0]=newaux[0];

    //printf("aux[0] = %c\n", aux[0]);
    int bla;

    //printf(".cod\n%s\n", newaux);
    //printf("length %d\n", strlen(newaux));
    //printf("aux = %s\n", aux);
    int h;
    char nbuffer[blockSize];
    int index = 0;
    
    for (i = 0, j = 1; i < strlen(newaux); i++, j++) {     //newaux[1,0,1,0,1,0,1,0]     aux[1,0] aux[10]   [1                i=1
        bla = atoi(aux);
        //printf("bla = %d\n", bla);
        h = findSF(str, bla, 256);
        //printf("%d\n", h);
        if (h != -2) {
            //printf("bla - %d = %c\n", bla, h);
            fputc(h, file);
            //printf("letra - %c\n", h);
            //printf("%c\n", nbuffer[index]);
            //printf("%d\n", h);
            aux[0] = '\0';
            j = 0;
            aux[j] = newaux[i + 1];
            aux[j+1] = '\0';
        }
        else {
            aux[j] = newaux[i+1];
            aux[j+1] = '\0'; 
        }    
    }

    //fwrite(nbuffer, sizeof(char), blockSize, file);
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

    for (i = 0; i < rlesize; i++){
        if (newbuffer[i] == 0){
            for (j = 0; j < newbuffer[i + 2]; j++){
                //if (!(newbuffer[i+1] > 96 && newbuffer[i] < 101)) printf("%d ", i); 
                fputc(newbuffer[i+1], file); 
            }
            i+=2;         
        }
        else fputc(newbuffer[i], file);
    }
}
