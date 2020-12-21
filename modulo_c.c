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
void readCod(FILE *cod,FILE *file,FILE* shaf,int *indexPointer,int *endFilePointer,int firstTime,int blockIndex);

//Função que lê do ficheiro o bloco e escreve a matriz dos codigos
void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int *index, int symbol,char *code,char* string);

void printMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE]);

void writeBlockToShaf (char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int blockSize,int blockNumber,int firstTime,FILE *cod,FILE *file,FILE *shaf);

int main(int argc, char *argv[]){

    if(argc<2){
        printf("O número de argumentos não é correto\nSintaxe: ./shafa [opcoes] (ficheiro a ser comprimido)\n"); 
        printf("Opcoes: \n -v verbose\n -m modulo\n");
        exit(1);
    }
    char fileFilename[strlen(argv[1])+1];
    

    
    strcpy(fileFilename,argv[1]);
    char codFilename[strlen(fileFilename)+9];
    strcpy(codFilename,fileFilename);
    strcat(codFilename,".cod");
    //Indice que guarda a posição lida no ficheiro .cod
    int index = 0;

    //Bool que diz se o ficheiro .cod chegou ao fim
    int endFile = 0;

    //Apontador para o o bool endFile
    int *endFilePointer = &endFile;

    //Apontador para a a variavel index
    int *indexPointer = &index;

    //Variavel que guarda o tempo do clock
    struct timespec t;

    clock_gettime(CLOCK_REALTIME,&t);

    //Parte inteira em segundos do tempo inicial
    int initialTimeSec = t.tv_sec;

    //Parte decimal do tempo inicial em nanossegundos
    int initialTimeNSec = t.tv_nsec;

    //Parte inteira em segundos do tempo inicial
    int finalTimeSec;
    
    //Parte decimal do tempo inicial em nanossegundos
    int finalTimeNSec;

    FILE *file = fopen(fileFilename,"r");
    FILE *cod = fopen(codFilename,"r");

    if(file == NULL) {
        printf ("Erro: Ficheiro original inexistente!\n");
        exit(2);
    }
    if(cod == NULL){
        printf ("Erro : Ficheiro .cod inexistente!\n");
        exit(3);
    }

    char shafFilename[strlen(fileFilename)+6];
    strcpy(shafFilename,fileFilename);

    FILE *shaf = fopen(strcat(shafFilename,".shaf"),"w");

    printf("Miguel Santa Cruz e Marisa Soares, MIEI/CD, 2020\n");
    printf("Modulo C - Codificação do ficheiro original/RLE\n");
    int firstTime = 1;

    //Ler o ficheiro .cod, codificar os blocos e escrever para o ficheiro .shaf
    for (int i = 0; *endFilePointer != 1; i++){
        readCod(cod,file,shaf,indexPointer,endFilePointer,firstTime,i);
        fseek(cod,index,SEEK_SET);
        firstTime = 0;
    }

    clock_gettime(CLOCK_REALTIME,&t);
    finalTimeSec = t.tv_sec;
    finalTimeNSec = t.tv_nsec;
    printf("Tempo total : %.1f ms\nFalta tambem a taxa de compressão de cada bloco e taxa global de compressao\n",(finalTimeSec-initialTimeSec)*1e3+(finalTimeNSec-initialTimeNSec)/1e6);
    return 0;
}

void readCod(FILE *cod,FILE *file,FILE* shaf,int *indexPointer,int *endFilePointer,int firstTime,int blockIndex){
    
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
    index++;//Salta o primeiro '@'
    
    int constant;
    if(firstTime){
        //Ler a informação ficheiro original/RLE
        if(string[index++]=='N') rOrN = 1; 

        //Debugging ---------------------------------------
        //printf("R(0) ou N(1): %d\n",rOrN);
        //-------------------------------------------------

        //Ler o número de blocos e guardar no array blockNumber
        constant = index+1;
        for (int i = constant; string[i]!='@'; i++){
            buffer[i-constant] = string[i];
            index = i;
        }
        blockNumber = atoi(buffer);
        printf("Número de blocos: %d\n",blockNumber);

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
    printf("Tamanho do %dº bloco: %d bytes\n",blockIndex+1,blockSize);
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
    printMatrix(codesMatrix);
    writeBlockToShaf(codesMatrix,blockSize,blockNumber,firstTime,cod,file,shaf);
    
    return;
}

void writeBlockToShaf (char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int blockSize,int blockNumber,int firstTime,FILE *cod,FILE *file,FILE *shaf){
    char buffer[blockSize+1];
    unsigned char binaryCodes[CODE_SIZE*blockSize+15]; //A constante 15 pode ser qualquer valor (espaço para guardar o tamanho do bloco)
    unsigned char code[9];
    int count = 0;
    int indexCode = 0;
    if (firstTime) sprintf(binaryCodes,"@%d@%d@",blockNumber,blockSize);
    else sprintf(binaryCodes,"@%d@",blockSize);
    if (firstTime) indexCode = 7;
    indexCode = strlen(binaryCodes);
    fread(buffer,sizeof(char),blockSize,file);
    int n = 0;
    //Escrever para binaryCodes a tradução do ficheiro original
    for (int i = 0 ; i<blockSize+1; i++) {
        //printf("%d: %c\n",buffer[i],buffer[i]);
        for (int j = 0; codesMatrix[buffer[i]][j]!='\0'; j++){
            if(n > 7) {
                binaryCodes[indexCode]= strtol(code,NULL,2);
                //printf("%lu\n",strtol(code,NULL,2));
                indexCode++;
                n = 0;
                code[n++]=codesMatrix[buffer[i]][j];
            } else code[n++]=codesMatrix[buffer[i]][j];
        }
        count++;  
    }
    printf("Tamanho comprimido: %d bytes\n",indexCode);
    printf("Taxa de compressão: %.1f %%\n",((float)(blockSize-indexCode)/blockSize)*100);
    fwrite(binaryCodes,1,indexCode,shaf);
    return;  
}

void printMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE]){
    for (int i = 0; i < NUMBER_OF_SYMBOLS-1; i++)
    {
        if (codesMatrix[i][0]!='\0') {
            printf("%d: ",i);
            for (int j = 0; codesMatrix[i][j]!='\0'; j++)
            {
                printf("%c ",codesMatrix[i][j]);
            }  
        printf("\n");
        } 
    }
}

void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE],int *indexPointer, int symbol,char *code,char* string){
    int index = *indexPointer;
    int constant = index+1;
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
    codesMatrix[254][codeBit]='\0';
    *indexPointer = index;
    return;
}