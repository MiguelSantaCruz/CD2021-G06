#include "modulo_c.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int mainModuloC(int argc, char *argv[]){
    positionIndicator posIndicator = malloc(sizeof(struct positionIndicator));
    posIndicator->codIndex = 0;

    fileData fData = malloc(sizeof(struct fileData));
    initializeStructs(&fData,&posIndicator); 
    commandLine(argc,argv,&fData);

    //Variaveis que guardam o tempo do clock
    struct timespec initialTime;
    struct timespec finalTime;
    clock_gettime(CLOCK_REALTIME,&initialTime);
    //Matriz dos códigos de cada símbolo
    char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1];
    //char* codBuffer = malloc((READ_SIZE*sizeof(char))+1);
    char codBuffer[READ_SIZE+1];
    fread(codBuffer,sizeof(char),READ_SIZE,fData->cod);
    //Ler o ficheiro .cod, codificar os blocos e escrever para o ficheiro .shaf
    for (int i = 0; fData->endOfFile != 1 && (fData->blockIndex < fData->blockNumber); i++) readCodFile(codesMatrix,codBuffer,&posIndicator,&fData);
    clock_gettime(CLOCK_REALTIME,&finalTime);
    printRunDetails(&fData,&initialTime,&finalTime);
    fclose(fData->cod);
    fclose(fData->file);
    fclose(fData->shaf);
    return 0;
}

void readCodFile(char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1],char* buffer,positionIndicator* posIndicator,fileData* fData){
    readToBufferIfNeeded(fData,posIndicator,buffer);
    readFileandBlocksInfo(posIndicator,fData,buffer);
    readAndWriteToMatrix(codesMatrix,fData,posIndicator,buffer);
    if ((*fData)->verbose) printMatrix(codesMatrix);
    (*posIndicator)->codIndex--;
    writeBlockToShaf(codesMatrix,fData);
    return;
}

void readFileandBlocksInfo(positionIndicator* posIndicator,fileData* fData,char* buffer){
    //Buffer que guarda informação temporária
    char tempBuffer[BUF_SIZE];
    (*fData)->blockIndex++;
    (*posIndicator)->codIndex++; //Salta o primeiro '@'
    if((*fData)->firstTime){
         (*fData)->blockIndex = 1;
        readToBufferIfNeeded(fData,posIndicator,buffer);
        //Ler a informação ficheiro original/RLE
        if(buffer[(*posIndicator)->codIndex++]=='N') (*fData)->rle = 1;
        if ((*fData)->verbose) (*fData)->rle ? printf("Compressão RLE: Não\n") : printf("Compressão RLE: Sim\n");
        //Ler o número de blocos e guardar no array blockNumber
        int index = 0;
        for (int i = ++((*posIndicator)->codIndex); buffer[i]!='@'; i++){
            readToBufferIfNeeded(fData,posIndicator,buffer);
            tempBuffer[index++] = buffer[i];
            (*posIndicator)->codIndex = i;
        }
        (*fData)->blockNumber = atoi(tempBuffer);
        printf("Número de blocos: %d\n",(*fData)->blockNumber);
        printf("-------------------------------------------\n");
    }
    //Reiniciar buffer
    for (int i = 0; i<=BUF_SIZE;i++) tempBuffer[i]='\0';
    //Ler o tamanho do 1º bloco e guardar no array blockSize
    readToBufferIfNeeded(fData,posIndicator,buffer);
    if (buffer[(*posIndicator)->codIndex]=='@')(*posIndicator)->codIndex++;
    else (*posIndicator)->codIndex+=2;
    int index = 0;
    for (int i = (*posIndicator)->codIndex; buffer[i] != '@'; i++){
        (*posIndicator)->codIndex = i;
        if(readToBufferIfNeeded(fData,posIndicator,buffer)) i = 0;
        tempBuffer[index++]=buffer[i];
        if (index>BUF_SIZE) printError(-5);
        (*posIndicator)->codIndex = i;
    }
    if((*fData)->endOfFile) return;
    (*posIndicator)->codIndex++;
    (*fData)->blockSize = atoi(tempBuffer);
    (*fData)->fileSize += (*fData)->blockSize;
    if((*fData)->blockSize) printf("Tamanho do %dº bloco: %d bytes\n",(*fData)->blockIndex,(*fData)->blockSize);
    if (!(*fData)->blockSize) (*fData)->endOfFile = 1;
    return;
}

int readToBufferIfNeeded(fileData* fData,positionIndicator* posIndicator,char* buffer){
    int refreshNeed = 0;
    if((*posIndicator)->codIndex >= READ_SIZE){
        if(!feof((*fData)->cod))fread(buffer,sizeof(char),READ_SIZE,(*fData)->cod);
        else (*fData)->endOfFile = 1;
        (*posIndicator)->codIndex = 0;
        refreshNeed = 1;
    }
    return refreshNeed;
}

void writeBlockToShaf(char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1],fileData* fData){
    //buffer de leitura do ficheiro original
    unsigned char* buffer = (char*) malloc(((*fData)->blockSize+1)*sizeof(char));

    //Array que contem a codificação binária a escrever no ficheiro .shaf
    //A constante 50 pode ser qualquer valor (espaço para guardar o tamanho do bloco)
    char* binaryCodes = (char* ) malloc((CODE_SIZE*(*fData)->blockSize)*sizeof(char));

    //Array para agrupar 8 bits de cada vez para depois guardar no array binaryCodes
    char code[9];

    //Buffer que será escrito no ficheiro (codificação binária + informação sobre os blocos)
    char* bufferOut = (char* )malloc((CODE_SIZE*(*fData)->blockSize)*sizeof(char));

    //Indice de leitura do array code
    int indexCode = 0;

    fread(buffer,sizeof(char),(*fData)->blockSize,(*fData)->file);

    //Indice de leitura do array code
    int codeBit = 0;
    for (int i = 0; i<8;i++) code[i] = 0;
    code[8] = '\0';
    //Escrever para binaryCodes a tradução do ficheiro original
    for (int i = 0; i<(*fData)->blockSize; i++){
        int c = (unsigned) buffer[i];
        for (int j = 0; codesMatrix[c][j]!='\0'; j++){
            if(codeBit>7) {
                binaryCodes[indexCode++]= strtol(code,NULL,2);
                //printf(" - convertido para: %ld \n",strtol(code,NULL,2));
                codeBit = 0;
                }
            code[codeBit++]=codesMatrix[buffer[i]][j];
        }
    }
    //Padding
    if(codeBit!=0) {
        for (;codeBit<8;codeBit++) code[codeBit] = '0';
        binaryCodes[indexCode++]= strtol(code,NULL,2);
    }
    //Variavel que indica o indice de leitura no array bufferOut
    int bufferOutIndex = 0;

    if ((*fData)->firstTime) sprintf(bufferOut,"@%d@%d@",(*fData)->blockNumber,indexCode);
    else sprintf(bufferOut,"@%d@",indexCode);
    (*fData)->firstTime = 0;
    bufferOutIndex = strlen(bufferOut);
    (*fData)->compressedFileSize += bufferOutIndex;
    for(int i = 0; i < 8 ;i++) code[i] = 0;
    printf("Tamanho do bloco comprimido: %d bytes\n",indexCode+1);
    (*fData)->compressedFileSize  += indexCode;
    printf("Taxa de compressão: %.1f %%\n",((float)((*fData)->blockSize-(indexCode+1))/(*fData)->blockSize)*100);
    for (int i = bufferOutIndex; i<=CODE_SIZE*((*fData)->blockSize-bufferOutIndex);i++) bufferOut[i]=binaryCodes[i-bufferOutIndex];
    fwrite(bufferOut,1,indexCode+bufferOutIndex,(*fData)->shaf);
    printf("-------------------------------------------\n");
    free(binaryCodes);
    free(buffer);
    free(bufferOut);
    return;
}


void printMatrix(char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1]){
    printf("Matriz dos códigos:\nSímbolo | Código\n");
    for (int i = 0; i < NUMBER_OF_SYMBOLS; i++) {
        if (codesMatrix[i][0]!='\0') {
            printf("%d: ",i);
            for (int j = 0; codesMatrix[i][j]!='\0'; j++) printf("%c",codesMatrix[i][j]);
        printf("\n");
        }
    }
}

void readAndWriteToMatrix(char codesMatrix[NUMBER_OF_SYMBOLS+1][CODE_SIZE+1],fileData* fData,positionIndicator* posIndicator,char* buffer){
    //Array que guarda a sequencia de bits correspondente a cada simbolo
    char code [CODE_SIZE];
    //Numero do simbolo
    int symbol = 0;
    //Indice de leitura do array code
    int codeBit = 0;
    readToBufferIfNeeded(fData,posIndicator,buffer);
    for (int i = ((*posIndicator)->codIndex+1); buffer[i]!='@' && symbol<=255; i++){
        (*posIndicator)->codIndex+=1;
        if(readToBufferIfNeeded(fData,posIndicator,buffer)) i = 0;
        for (int j = 0; buffer[i]!=';' && buffer[i]!='@'; j++){
            if(readToBufferIfNeeded(fData,posIndicator,buffer)) i = 0;
            code[j]=buffer[i++];
            (*posIndicator)->codIndex++;
            codeBit++;
            if(readToBufferIfNeeded(fData,posIndicator,buffer)) i = 0;
        }
        if (codeBit) {
            code[codeBit]='\0';
            int k = 0;
            for (; code[k]!='\0'; k++) codesMatrix[symbol][k]=code[k];
            codesMatrix[symbol][k]='\0';
        } else codesMatrix[symbol][0]='\0';
        symbol++;
        codeBit = 0;
        (*posIndicator)->codIndex = i;
    }
    
    if (codeBit) {
            code[codeBit]='\0';
            int k = 0;
            for (; code[k]!='\0'; k++) codesMatrix[symbol][k]=code[k];
            codesMatrix[symbol][k]='\0';
    } else codesMatrix[symbol][0]='\0';
    return;
}

void commandLine(int argc,char* argv[],fileData* fData){
    if(argc < 4 || argc > 5) printError(-1);
    if (strcmp(argv[2],"-m") || strcmp(argv[3],"c")) printError(-2);
    if (argc == 5 && (strcmp(argv[4],"-v"))) printError(-2);
    else if (argc == 5) (*fData)->verbose = 1;
    openFiles(argv,fData);
    printf("Miguel Santa Cruz e Marisa Soares, MIEI/CD, 2020\n");
    printf("Modulo C - Codificação do ficheiro original/RLE\n");
    if ((*fData)->verbose) printf("[Verbose mode]\n");
    return;
}

void openFiles(char* argv[],fileData* fData){
    char fileFilename[strlen(argv[1])];
    strcpy(fileFilename,argv[1]);
    char codFilename[strlen(fileFilename)+5];
    strcpy(codFilename,fileFilename);
    strcat(codFilename,".cod");
    (*fData)->file = fopen(fileFilename,"r");
    (*fData)->cod = fopen(codFilename,"r");
    if(!(*fData)->file  || !(*fData)->cod) printError(-3);
    char shafFilename[strlen(fileFilename)+6];
    strcpy(shafFilename,fileFilename);
    (*fData)->shaf = fopen(strcat(shafFilename,".shaf"),"w");
    strcat((*fData)->shafFilename,shafFilename);
    if(!(*fData)->shaf) printError(-4);
    return;
}

void printError(int x){
    switch (x){
    case -1:
        printf("O número de argumentos não é correto\nSintaxe: ./shafa (ficheiro a ser comprimido) -m c [opcoes]\n");
        exit(-1);
        break;
    case -2:
        printf("Opção desconhecida\nSintaxe: ./shafa (ficheiro a ser comprimido) -m c [opções]\n");
        printf("Opções disponiveis:\n - v : verbose mode\n");
        exit(-2);
        break;
    case -3:
        printf ("Erro: Ficheiro original ou ficheiro .cod inexistente !\n");
        exit(-3);
        break;
    case -4:
        printf ("Erro: Não foi possível criar o ficheiro .shaf!\n");
        exit(-4);
        break;
    case -5:
        printf ("Erro:Tamanho do bloco muito grande\n");
        exit(-5);
        break;
    case -6:
        printf("Erro\nSintaxe: ./shafa (ficheiro a ser comprimido) -m [f|t|c|d] [opções]\n");
        printf("Opções disponiveis:\n - v : verbose mode\n");
        exit(-6);
        break;
    default:
        printf("Erro inesperado\n");
        break;
    }
    return;
}

void printRunDetails(fileData* fData,struct timespec* initialTime,struct timespec* finalTime){
    printf("Tamanho original do ficheiro: %d bytes\n",(*fData)->fileSize);
    printf("Tamanho do ficheiro comprimido: %d bytes\n",(*fData)->compressedFileSize);
    printf("Taxa de compressão total: %.1f %%\n",((float)((*fData)->fileSize)-(*fData)->compressedFileSize)/((*fData)->fileSize)*100);
    printf("Tempo total : %.1f ms\n",(finalTime->tv_sec-initialTime->tv_sec)*1e3+(finalTime->tv_nsec-initialTime->tv_nsec)/1e6);
    printf("Ficheiro de saída: %s\n",(*fData)->shafFilename);
    return;
}

void initializeStructs(fileData* fData,positionIndicator* posIndicator){
    (*fData)->verbose = 0;
    (*fData)->blockNumber = 2; //Inicializado a 2 para ser maior que blockIndex (será substituido logo na primeira execução)
    (*fData)->blockSize = 0;
    (*fData)->blockIndex = 1;
    (*fData)->endOfFile = 0;
    (*fData)->firstTime = 1;
    (*fData)->rle = 0;
    (*fData)->fileSize = 0;
    (*fData)->compressedFileSize = 0;
    (*posIndicator)->codIndex = 0;
    return;
}
