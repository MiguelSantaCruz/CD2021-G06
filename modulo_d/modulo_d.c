#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "modulo_d.h"

#define CODE_SIZE 16
#define NUMBER_OF_SYMBOLS 256

int mainModuloD (int argc, char* argv[]){
    int firsttime = 1;
    int indexpointerCod = 0; //Apontador do ficheiro .cod
    int indexpointerShaf = 0; //Apontador do ficheiro .shaf  
    int filesize;
    //
    clock_t begin = clock();
    //Lê ficheiros .shaf e .cod
    if ((strcmp(argv[2], "-m")==0) && (strcmp(argv[3], "d")==0)){
        printf("\nArmando Silva - A87949 , Benjamim Costa A87985\n");
        printf("MIEI/CD, 2020\n");
        printf("Módulo D: Descodificação do ficheiro comprimido\n");
        if(!(argv[4] && argv[5])){
            char NorR;
            char bufferTemp[3];
        
            char* shafFilename;
            shafFilename = argv[1];
            char* codFilename = (char*) malloc(strlen(shafFilename)-1);
            memcpy(codFilename,shafFilename,strlen(shafFilename)-4);
            strcat(codFilename,"cod");

            

            FILE *shaf = fopen(shafFilename,"rb");
            FILE *cod = fopen(codFilename,"r");
            fread(bufferTemp,1,3,cod);
            sscanf(bufferTemp,"@%c@%d@",&NorR);
            fseek(cod,0,SEEK_SET);

            if(NorR== 'N'){
                char* filename = (char*) malloc(strlen(codFilename)-4);
                memcpy(filename,codFilename,strlen(codFilename)-4);  

                FILE *file = fopen(filename, "w");
            
                if(shaf == NULL) {
                    printf ("Erro: Ficheiro original inexistente!\n");
                    exit(2);
                }
                if(cod == NULL){
                    printf ("Erro : Ficheiro .cod inexistente!\n");
                    exit(3);
                }
                char bufferTemp[10];
                int numBlocks;
                fread(bufferTemp,1,10,shaf);
                sscanf(bufferTemp,"@%d@",&numBlocks);
                fseek(shaf,0,SEEK_SET);
                printf("Número de blocos: %d\n", numBlocks);
                
                for(int t=0;t < numBlocks;t++)
                    readShaf(shaf,cod,file,&firsttime,&indexpointerCod,&indexpointerShaf);
                
                clock_t end = clock(); 
                float final = ((float)(end - begin))*1000/ CLOCKS_PER_SEC;
                printf("Tempo de execução do módulo: %.2f ms\n",final);
                printf("Ficheiro gerado: %s\n\n", filename);

                
                }
            if(NorR=='R'){
                char* filename = (char*) malloc(strlen(codFilename)-8);
                memcpy(filename,codFilename,strlen(codFilename)-8);
                


                char* freqfile = (char*) malloc(strlen(codFilename)-8);
                memcpy(freqfile,codFilename,strlen(codFilename)-8);
                
                FILE *file= fopen(filename, "w"); // .txt
                char* finalfile = (char*) malloc(strlen(codFilename)-8);
                strcpy(finalfile, filename);
                strcat(filename,".rle");

                strcat(freqfile,".freq");
                FILE *freq = fopen(freqfile, "r"); // .freq
                FILE *tmp = fopen(filename, "w"); // .rle
                
                

                char bufferTemp[10];
                int numBlocks;
                fread(bufferTemp,1,10,shaf);
                sscanf(bufferTemp,"@%d@",&numBlocks);
                fseek(shaf,0,SEEK_SET);
                printf("Número de blocos: %d\n", numBlocks);
                
                for(int t=0;t < numBlocks;t++)
                    readShaf(shaf,cod,tmp,&firsttime,&indexpointerCod,&indexpointerShaf);
                
                fclose(shaf);
                fclose(cod);
                fclose(tmp);
                FILE *tmp1 = fopen(filename, "r");

                filesize= contaFilesizeRle(freq);
                readRLE(tmp1,freq,file,filesize);
                fclose(freq);
                fclose(tmp1);
                remove(filename);


                clock_t end = clock(); 
                float final = ((float)(end - begin))*1000/ CLOCKS_PER_SEC;
                printf("Tempo de execução do módulo: %.2f ms\n",final);
                printf("Ficheiro gerado: %s\n\n", finalfile);
                
            }
        }
        if (argv[4] && argv[5]){
            if ((strcmp(argv[4], "-d")==0) && (strcmp(argv[5], "s")==0)){
                char NorR;
                char bufferTemp1[3];
            
                
                char* shafFilename;
                shafFilename = argv[1];
                char* codFilename = (char*) malloc(strlen(shafFilename)-1);
                memcpy(codFilename,shafFilename,strlen(shafFilename)-4);
                strcat(codFilename,"cod");
        
                FILE *shaf = fopen(shafFilename,"rb");
                FILE *cod = fopen(codFilename,"r");
                fread(bufferTemp1,1,3,cod);
                sscanf(bufferTemp1,"@%c@",&NorR);
                fseek(cod,0,SEEK_SET);
                
                if(NorR == 'N'){
                    printf("Erro! Este ficheiro não está comprimido em RLE.\nTente ./shafa filename -m d");
                    return 0;
                }

                char* filename = (char*) malloc(strlen(codFilename)-4);
                memcpy(filename,codFilename,strlen(codFilename)-4);
                printf("%s\n", filename);

                FILE *rle = fopen(filename, "w"); //rle
                

                char bufferTemp[10];
                int numBlocks;
                fread(bufferTemp,1,10,shaf);
                sscanf(bufferTemp,"@%d@",&numBlocks);
                fseek(shaf,0,SEEK_SET);
                printf("Número de blocos: %d\n", numBlocks);
                
                for(int t=0;t < numBlocks;t++)
                    readShaf(shaf,cod,rle,&firsttime,&indexpointerCod,&indexpointerShaf);

                clock_t end = clock(); 
                float final = ((float)(end - begin))*1000/ CLOCKS_PER_SEC;
                printf("Tempo de execução do módulo: %.2f ms\n",final);
                printf("Ficheiro gerado: %s\n\n", filename);
            
            }
  
            
            
            //Read RLE
            if ((strcmp(argv[4], "-d")==0) && (strcmp(argv[5], "r")==0)){
                
                char* rleFilename;
                rleFilename = argv[1];
                char* freqFilename = (char*) malloc(strlen(rleFilename)-1);
                memcpy(freqFilename,rleFilename,strlen(rleFilename)-3);
                strcat(freqFilename,"freq");
                char* filetxt = (char*) malloc(strlen(freqFilename)-5);
                memcpy(filetxt,freqFilename,strlen(freqFilename)-5);


                FILE *rle = fopen(rleFilename,"r");
                FILE *freq = fopen(freqFilename,"r");
                FILE *filerle = fopen(filetxt, "w");

                if(rle == NULL) {
                    printf ("Erro: Ficheiro original inexistente!\n");
                    exit(2);
                }
                if(freq == NULL){
                    printf ("Erro : Ficheiro .freq inexistente!\n");
                    exit(3);
                }

                char bufferTemp[10];
                int numBlocks;
                char type;
                fread(bufferTemp,1,10,freq);
                sscanf(bufferTemp,"@%c@%d@",&type, &numBlocks);
                fseek(freq,0,SEEK_SET);
                printf("Número de blocos: %d\n", numBlocks);

                filesize= contaFilesizeRle(freq);
                readRLE(rle,freq,filerle,filesize);
                clock_t end = clock(); 
                float final = ((float)(end - begin))*1000/ CLOCKS_PER_SEC;
                printf("Tempo de execução do módulo: %.2f ms\n",final);
                printf("Ficheiro gerado: %s\n\n", filetxt);

            }

        }
        
    

    }

    if (argc > 6){
        printf("O número de argumentos não é correto\nSintaxe: ./shafa (ficheiro a ser descomprimido) -m d\n");
        exit(1);
    }
       
    return 0;    
}
void readShaf(FILE *shaf, FILE *cod, FILE *file,int* firsttime, int* indexpointerCod, int* indexpointerShaf){

    unsigned char bufferShaf[30];
    unsigned char nextbufferShaf[65000];
    int i,j;
    int numblocks, blockSize;
    int firstaux;  //Inteiro que ajuda na mudança de apontadores no ficheiro shaf
    
    //Leitura para definir o numero de blocos e o tamanho do bloco
    fread(bufferShaf,1,30,shaf);

    firstaux = *firsttime;

    if(*firsttime){
        sscanf(bufferShaf,"@%d@%d@",&numblocks,&blockSize);
    }
    else sscanf(bufferShaf,"@%d@",&blockSize);

    //muda o apontador do ficheiro para o inicio do conteúdo do bloco
    if(*firsttime) 
        fseek(shaf,contaDigitos(blockSize)+contaDigitos(numblocks)+3, SEEK_SET);    
    else fseek(shaf, *indexpointerShaf + contaDigitos(blockSize)+2, SEEK_SET);

    //Leitura do "conteudo" do bloco
    fread(nextbufferShaf,1, blockSize, shaf);


    int bin; //Número binário

    char binShaf[blockSize*16]; //Armaneza os bits
    binShaf[0] = '\0';
   
    //Mete num array bit a bit
    for(i = 0; i<blockSize; i++){
        char aux[64];
        bin = decimalToBinary(charToInt(nextbufferShaf[i]));    
        sprintf(aux, "%d", bin); //coloca binário na String
        strcat(binShaf, aux); //vai adicionando cada inteiro em bit
        aux[0] = '\0';
    }


    
    //Avança o apontador do ficheiro Shaf para o inicio do próximo bloco, i.e @tamanho_bloco@conteudo_do_bloco[...]
    *indexpointerShaf += i;
    if (firstaux){
        *indexpointerShaf += contaDigitos(blockSize)+contaDigitos(numblocks)+3;
        fseek(shaf, *indexpointerShaf, SEEK_SET);
    }    
    else  fseek(shaf, *indexpointerShaf + contaDigitos(blockSize) + 2, SEEK_SET);  
    

    readCod(cod,binShaf,file,firsttime,indexpointerCod);
     
}

int findSF(char str[NUMBER_OF_SYMBOLS][CODE_SIZE], char aux[CODE_SIZE]){      
    int i,j;
    int certo;
    for (i = 0; i < 256; i++){
        if (str[i][0] == aux[0])
            for (j = 0, certo = 0; str[i][j] != '2'; j++)
                if (str[i][j] == aux[j]) certo++;        
            if (j == certo) return i;                                                       
    }
    return -1;    
}


int charToInt(unsigned char *c){
    int i;
    i = (int)(c);
    return i;
}


int  decimalToBinary(int decimalnum){
    int binarynum = 0;
    int rem, temp = 1;

    while (decimalnum!=0){
        rem = decimalnum%2;
        decimalnum = decimalnum / 2;
        binarynum = binarynum + rem*temp;
        temp = temp * 10;
    }
    return binarynum;
}

                        
void readCod(FILE *cod, char* binShaf, FILE* txt,int* firsttime, int* indexpointerCod){

    unsigned char bufferCod[21];
    unsigned char nextbuffer[7001];
    int i,j,k;
    int numBlocks,blockSize;
    int firstaux; //Inteiro que ajuda na mudança de apontadores no ficheiro cod
    char compressType;

    //Leitura para definir o numero de blocos e o tamanho do bloco 
    fread(bufferCod,1,20,cod);

    firstaux = *firsttime;

    if(*firsttime){ 
        sscanf(bufferCod,"@%c@%d@%d@",&compressType,&numBlocks,&blockSize);
    }
    else sscanf(bufferCod,"@%d@",&blockSize);

    //muda o apontador do ficheiro para o inicio do conteúdo do bloco
    if(*firsttime){
        fseek(cod,contaDigitos(blockSize)+contaDigitos(numBlocks)+5,SEEK_SET);
        *firsttime=0;
        }
    else fseek(cod,*indexpointerCod + contaDigitos(blockSize)+2,SEEK_SET);

    //Leitura do "conteudo" do bloco 
    fread(nextbuffer,1,7000,cod);

    char str[256][16]; //Matriz que armazena os códigos SF onde cada posição corresponde ao número da Tabela ASCII
    int pos,m; //apontadores Matriz[pos][m]

    //Posições que não têm código SF contêm '2'
    for(i = 0; i<256; i++) 
        for (j = 0; j < 16; j++)
            str[i][j] = '2';

    for (i = 0, pos = 0; pos < 256; i++, pos++){   
        char aux[16]; //auxiliar que armazena cada digito de um código SF
        j = 0;
        while(nextbuffer[i] != ';' && nextbuffer[i]!='@') {
            aux[j++] = nextbuffer[i];
            i++;
        }
        aux[j] = '\0';
        
        if (j > 0)
            for(m = 0; m < j; m++)
                str[pos][m] = aux[m];
    }

    //Avança o apontador do ficheiro Cod para o inicio do próximo bloco, i.e @tamanho_bloco@conteudo_do_bloco[...]
    *indexpointerCod += i;
    if (firstaux){
        *indexpointerCod += contaDigitos(blockSize)+contaDigitos(numBlocks)+4;
        fseek(cod, *indexpointerCod, SEEK_SET);
    }    
    else fseek(cod, *indexpointerCod + contaDigitos(blockSize)+2, SEEK_SET);   
    

    char auxx[CODE_SIZE]; //String que armazena cada bit 1 a 1 até encontrar um código SF igual ao mesmo na String "str"
    
    for (i = 0; i < 16; i++)   
        auxx[i] = '\0';
    auxx[0]=binShaf[0];
  
    int h; //Posição da tabela ASCII 
    
    for (i = 0, j = 1, k = 0, pos = 0; binShaf[i] != '\0' ; i++, j++) {    //binShaf[i] != '\0'
        h = findSF(str, auxx);
        if (h != -1) {
            fputc(h, txt);
            k++;
            auxx[0] = '\0';
            j = 0;
            auxx[j] = binShaf[i + 1];
            auxx[j+1] = '\0';
        }
        else {
            auxx[j] = binShaf[i+1];
            auxx[j+1] = '\0'; 
        }    
    }
}


void readRLE(FILE *rle, FILE *freq, FILE *txt, int rlesize){
    int i,j;
    
    char newbuffer[rlesize];

    fread(newbuffer, sizeof(char), rlesize, rle);

    for (i = 0; i < rlesize; i++){
        if (newbuffer[i] == 0){
            for (j = 0; j < newbuffer[i + 2]; j++)
                fputc(newbuffer[i+1], txt); 
            i+=2;         
        }
        else fputc(newbuffer[i], txt);
    }
}

int contaFilesizeRle(FILE *freq){
    //Conta '@'
    int contador=0;

    int blockSize,i,j;
    char buffer[65000];

    //Acumula cada digito do tamanho do RLE 1 a 1
    char buff[5];
    

    fread(buffer,sizeof(char),1800,freq);
    
    for (i = 0; contador < 3; i++){
        if (buffer[i] == '@'){
            contador++;
            if (contador == 3) {
                for ( j = 0, i += 1; buffer[i] != '@'; j++)
                    buff[j] = buffer[i++];
                        
                blockSize = atoi(buff);
                buff[0]='\0';
                contador=3;
            }  
        }
    } 
    
    //Atravessa o "conteúdo" do primeiro bloco
    for(j=i;buffer[j]!='@';j++);
        

    for(j,contador=0;buffer[j]!= '\0' ;j++){
        if(buffer[j]=='@'&& contador < 1){
            contador++;
            for(j+=1,i=0;buffer[j+1]!='@' ;j++)
                buff[i++]=buffer[j];
        
            blockSize += atoi(buff);
            j+=1;
            contador=0;
        }
    }    
    return blockSize;
}


int contaDigitos (int num){
    int count = 0;
    do {

        count++;
        num /= 10;
    
    } while(num != 0);
  
  return count;
}