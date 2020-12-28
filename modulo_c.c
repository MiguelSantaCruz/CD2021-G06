#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "modulo_c.h"

#define READ_SIZE 4081
#define BUF_SIZE 17
#define CODE_SIZE 16
#define NUMBER_OF_SYMBOLS 256

int main(int argc, char *argv[]){

    if(argc < 4 || argc > 5){
        printf("O número de argumentos não é correto\nSintaxe: ./shafa (ficheiro a ser comprimido) -m c [opcoes]\n");
        exit(1);
    }
    if (strcmp(argv[2],"-m") || strcmp(argv[3],"c")) {
        printf("Opção desconhecida\nSintaxe: ./shafa (ficheiro a ser comprimido) -m c [opcoes]\n");
        printf("Opções disponiveis:\n - v : verbose mode\n");
        exit(1);
    }
    int verbose = 0;
    if (argc == 5) {
        if (!(strcmp(argv[4],"-v"))) verbose = 1;
        else {
            printf("Opção desconhecida\nSintaxe: ./shafa (ficheiro a ser comprimido) -m c [opcoes]\n");
            printf("Opções disponiveis:\n - v : verbose mode\n");
            exit(1);
        }
    }
    char fileFilename[strlen(argv[1])];
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
    int fileSize = 0;
    int *fileSizePointer = &fileSize;
    int compressedBlockSize = 0;
    int *compressedBlockSizePointer = &compressedBlockSize;

    //Ler o ficheiro .cod, codificar os blocos e escrever para o ficheiro .shaf
    for (int i = 0; *endFilePointer != 1; i++){
        readCodFile(cod,file,shaf,indexPointer,endFilePointer,firstTime,i,compressedBlockSizePointer,fileSizePointer,verbose);
        fseek(cod,index,SEEK_SET);
        firstTime = 0;
    }

    clock_gettime(CLOCK_REALTIME,&t);
    finalTimeSec = t.tv_sec;
    finalTimeNSec = t.tv_nsec;
    printf("Tamanho do ficheiro comprimido: %d bytes\n",*compressedBlockSizePointer);
    printf("Tamanho de compressão total: %.1f %%\n",((float)(fileSize-compressedBlockSize)/fileSize)*100);
    printf("Tempo total : %.1f ms\n",(finalTimeSec-initialTimeSec)*1e3+(finalTimeNSec-initialTimeNSec)/1e6);
    printf("Ficheiro de saída: %s\n",shafFilename);
    return 0;
}

void readCodFile(FILE *cod,FILE *file,FILE* shaf,int *indexPointer,int *endFilePointer,int firstTime,int blockIndex,int* compressedBlockSizePointer,int* fileSizePointer,int verbose){
    //ponteiro para o indíce em que já foi lida a string lida do ficheiro
    int index = *indexPointer;

    //String que contem 4080 bytes lidos do ficheiro .cod
    char string[READ_SIZE];

    //Buffer que contem informaçao aleatória
    char buffer[BUF_SIZE];

    int blockNumber,blockSize;

    //Inteiro que guarda a R (0) ou N(1)
    int rOrN = 0;

    //Código do símbolo lido do ficheiro
    char code[CODE_SIZE+1];

    //Matriz dos códigos de cada símbolo
    char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1];

    //Obter os primeiros READ_SIZE bytes do ficheiro
    fgets(string,READ_SIZE,cod);
    index++;//Salta o primeiro '@'

    int constant;
    if(firstTime){
        //Ler a informação ficheiro original/RLE
        if(string[index++]=='N') rOrN = 1;

        //Debugging ---------------------------------------
        if (verbose) rOrN ? : printf("Ficheiro RLE\n");
        //-------------------------------------------------

        //Ler o número de blocos e guardar no array blockNumber
        constant = index+1;
        for (int i = constant; string[i]!='@'; i++){
            buffer[i-constant] = string[i];
            index = i;
        }
        blockNumber = atoi(buffer);
        printf("Número de blocos: %d\n",blockNumber);
        constant = index+2;
    }
    if(!firstTime) constant = ++index;

    //Ler o tamanho do 1º bloco e guardar no array blockSize
    for (int i = 0; i<=CODE_SIZE;i++) buffer[i]='\0';

    for (int i = constant; string[i] != '@'; i++){
        buffer[i-constant]=string[i];
        index = i;
    }
    index++;
    blockSize = atoi(buffer);
    *fileSizePointer += blockSize;
    if(blockSize) printf("Tamanho do %dº bloco: %d bytes\n",blockIndex+1,blockSize);
    if (blockSize == 0) {
        *endFilePointer = 1;
        return;
    }
    //Indice da sequencia de bits do simbolo
    int codeBit = 0;

    //Indice do símbolo na matriz
    int symbol = 0;
    *indexPointer = index;
    readAndWriteToMatrix(codesMatrix,indexPointer,string);
    if (verbose) printMatrix(codesMatrix);
    writeBlockToShaf(codesMatrix,blockSize,blockNumber,firstTime,compressedBlockSizePointer,cod,file,shaf);
    return;
}

void writeBlockToShaf (char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],int blockSize,int blockNumber,int firstTime,int* compressedBlockSizePointer,FILE *cod,FILE *file,FILE *shaf){
    //buffer de leitura do ficheiro original
    unsigned char buffer[blockSize];

    //Array que contem a codificação binária a escrever no ficheiro .shaf
    //A constante 50 pode ser qualquer valor (espaço para guardar o tamanho do bloco)
    unsigned char binaryCodes[CODE_SIZE*blockSize];

    //Array para agrupar 8 bits de cada vez para depois guardar no array binaryCodes
    unsigned char code[9];

    //Buffer que será escrito no ficheiro (codificação binária + informação sobre os blocos)
    unsigned char bufferOut[CODE_SIZE*blockSize+50];

    //Indice de leitura do array code
    int indexCode = 0;

    //Numero de bytes que contem informação sobre o bloco e não o bloco em si
    int blockInfo = 0;
    indexCode = 0;
    fread(buffer,sizeof(char),blockSize,file);
    //Indice de leitura do array code
    int n = 0;
    for (int i = 0; i<8;i++) code[i] = 0;
    code[8] = '\0';
    //Escrever para binaryCodes a tradução do ficheiro original
    for (int i = 0; i<blockSize; i++){
        for (int j = 0; codesMatrix[buffer[i]][j]!='\0'; j++){
            if(n>7) {
                binaryCodes[indexCode++]= strtol(code,NULL,2);
                n = 0;
                }
            code[n++]=codesMatrix[buffer[i]][j];
        }
    }
    //Padding
    if(n!=0) {
        for (n;n<8;n++) code[n] = '0';
        binaryCodes[indexCode++]= strtol(code,NULL,2);
    }
    int bufferOutIndex = 0;
    if (firstTime) sprintf(bufferOut,"@%d@%d@",blockNumber,indexCode);
    else sprintf(bufferOut,"@%d@",indexCode);
    bufferOutIndex = strlen(bufferOut);
    *compressedBlockSizePointer += bufferOutIndex;
    for(int i = 0; i < 8 ;i++) code[i] = 0;
    printf("Tamanho comprimido: %d bytes\n",indexCode+1);
    *compressedBlockSizePointer += indexCode;
    printf("Taxa de compressão: %.1f %%\n",((float)(blockSize-(indexCode+1))/blockSize)*100);
    for (int i = bufferOutIndex; i<=CODE_SIZE*blockSize-bufferOutIndex;i++) bufferOut[i]=binaryCodes[i-bufferOutIndex];
    fwrite(bufferOut,1,indexCode+bufferOutIndex,shaf);
    return;
}

void printMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1]){
    printf("Matriz --------------------------\n");
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++)
    {
        if (codesMatrix[i][0]!='\0') {
            printf("%d: ",i);
            for (int j = 0; codesMatrix[i][j]!='\0'; j++)
            {
                printf("%c",codesMatrix[i][j]);
            }
        printf("\n");
        }
    }
    printf("---------------------------------\n");
}

void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS][CODE_SIZE+1],int *indexPointer,char* string){
    char code[CODE_SIZE+1];
    int symbol = 0;
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
            int k = 0;
            for (k; code[k]!='\0'; k++) codesMatrix[symbol][k]=code[k];
            codesMatrix[symbol][k]='\0';
        } else codesMatrix[symbol][0]='\0';
        symbol++;
        index = i;
    }
    *indexPointer = index;
    return;
}
