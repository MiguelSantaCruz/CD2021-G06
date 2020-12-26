#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulo_b.h"
#include "fsize.h"

#define NUMBER_OF_FREQ 256



int main (int argc, char* argv[]){

    if(argc<2){
        printf ("Número de argumentos incorreto.\nUsar: ./shafa exemplo.txt.freq -m t");
        exit(1);
    }

    char fileName[strlen(argv[1])+1];
    strcpy(fileName,argv[1]);
    
    FILE * ficheiroFreq;
    ficheiroFreq = fopen (fileName, "rb");
    if (ficheiroFreq == NULL){
        printf ("ERRO: Ficheiro não existe!\n");
        exit(2);
    }

    unsigned long long total;
    long long n_blocks;
    unsigned long block_size;
    int size_of_last_block;

    fseek (ficheiroFreq, 0L, SEEK_SET);
    total = ftell(ficheiroFreq);
    rewind (ficheiroFreq);

    int index = 0;
    char c; 
    char a = '@';
    char *c_block_size;
    int arrobas = 0;
    int rOrN = 0; // N = 1 R = 0
    char str[12];
    int k = 1;

    while ((c = fgetc(ficheiroFreq)) != EOF){
        index++;
        if (c == 'N') rOrN = 1;

        if (a == c) arrobas++;

        else if (arrobas == 3){
            if (k==1){
                c_block_size = malloc(k*12);
                k++;
            }
            else {
                c_block_size = realloc(c_block_size, k*12);
                k++;
            }
            sprintf(str,"%d", atoi(&c));
            strcat (c_block_size, str);
        } else if (arrobas == 2) n_blocks = atoi(&c);

        if (arrobas == 4){
            break;
        }
    }
    int pos_freqs = index;
    index = index-k-1;
    fseek(ficheiroFreq, index, SEEK_SET);


    //rewind(ficheiroFreq);
    //fseek (ficheiroFreq, 0, SEEK_SET);
    printf("Block size str %s\n", c_block_size);
    block_size = atoi(c_block_size);
    printf("Block size %lu\n", block_size);
    //printf("Size of last block %ld\n", size_of_last_block);
    //n_blocks = fsize(ficheiroFreq, NULL, &block_size, &size_of_last_block);
    printf ("Total blocks %lld\n", n_blocks);
    //printf("Size of last block %ld\n", size_of_last_block);
    //total = (n_blocks-1) * block_size + size_of_last_block;

    //Função que le
    int freq_Array[256];
    char *codes, *final_codes;
    codes = (char *)malloc(256*256);
    final_codes = (char *)malloc(256);
    //freq_Array = (int *)malloc(total);
    memset(freq_Array,0,256);
    //memset(codes,-1,256);
    int i = 0;
    int m = 1;
    int dist = 0;
    int *indexpointer = &index;
    char *entry_buffer;
    entry_buffer = (char *)malloc(total);
    char temp;
    char strtemp[12];
    int array_size = 0;
    char *size_of_last_block_string;
    struct node nodes[256];
    memset (&nodes, -1 ,256*sizeof(node));
    

    while (i<n_blocks-1){
        dist = 256+distArroba (ficheiroFreq, 0, 0);
        fseek(ficheiroFreq, index, SEEK_SET);
        readFreqs(ficheiroFreq, indexpointer , entry_buffer, dist); //256 = distancia ate proxima arroba
        i++;
        indexpointer += dist;
        toStruct (nodes, entry_buffer, n_blocks, freq_Array, pos_freqs);
        minSort (freq_Array);
        /*
        int g=0;
        for (g =0; g<256;g+=1){
            printf("FREQB - %d\n",freq_Array[g]);
        }*/
        for(int f=0; f<256; f++){
            if(freq_Array[f]!=0) array_size++;
        }
        printf("ARRS-%d\n", array_size);
        
        char _matrix [NUMBER_OF_FREQ][NUMBER_OF_FREQ];
        calcular_codigos_SF (freq_Array, codes, _matrix, 0, array_size-1, 0);

        printf("\n");
        char d = ';';
        int cont=0;

        for(int i=0; i<NUMBER_OF_FREQ && i<array_size; i++){
            for(int j=0; j<NUMBER_OF_FREQ && j<array_size-1; j++){
                if(_matrix[i][j] =='0' || _matrix[i][j] =='1'){                
                    codes[cont]=_matrix[i][j];
                    cont++;
                    
                }
            }
            codes[cont]=d;
            cont++;
        }
        printf("CODES- %s\n", codes);
        int f,h, g = -1;
        char tmp[256];
        //for(int g = 0; g<sizeof(freq_Array)/sizeof(freq_Array[0]); g++){
        for (int z = 0; z<strlen(codes); z++){
            g++;
            for (int k = 0; k<256; k++){
                if (nodes[k].freq==freq_Array[g]){
                    f=z;
                    while (codes[f]!= ';'){
                        tmp[h]=codes[f];
                        h++;
                        f++;
                    }
                    z+=h;
                    tmp[h]='\0';
                    printf("TMP-%s\n",tmp);
                    strcpy (nodes[k].code, tmp);
                    printf("Freq - %d\n", nodes[k].freq);
                    printf("Code -%s\n",nodes[k].code);
                    h=0;
                    break;
                }
            }
        }
        
        
        for(int ab=0; ab<256; ab++){
            if(nodes[ab].init_symbol!=-1){
                printf("\n\nSymbI - %d\n", nodes[ab].init_symbol);
                printf("SymbF - %d\n", nodes[ab].final_symbol);
                printf("Freq - %d\n", nodes[ab].freq);
                printf("Cod - %s\n", nodes[ab].code);
            }
        }

        
    }

    printf ("\n\n\n1-%s\n\n\n", entry_buffer);
    printf("testAftewhile\n");
    int last_block_size = total - ftell(ficheiroFreq)-1;
    //dist =  last_block_size + distArroba(ficheiroFreq, last_block_size, 1);
    readFreqs(ficheiroFreq, indexpointer , entry_buffer, last_block_size);
    printf ("\n\n\n2-%s\n\n\n", entry_buffer);
    /*
    indexpointer += 256;
    temp = fgetc(ficheiroFreq);
    if (temp == '@'){
        while (fgetc(ficheiroFreq) != '@'){
            m++;
            sprintf(strtemp,"%d", atoi(&temp));
            strcat (size_of_last_block_string, strtemp);
        }
    }
    printf("testAftewhile2\n");
    size_of_last_block = atoi(size_of_last_block_string);
    indexpointer += m;*/
    /*for (int j=0; i<100;i++){
        printf("%c", entry_buffer[i]);
    }*/
    printf("testAftewhile3\n");
    //size_of_last_block = fileSize - index - (n_blocks-1 * 256);
    readFreqs (ficheiroFreq, indexpointer, entry_buffer, size_of_last_block);


    printf("%s\n","abc6");
    fclose (ficheiroFreq);
    free(entry_buffer);
    free(c_block_size);
    free(final_codes);
    printf("%s\n","abc7");

    //FUnção que shano
    //Função que escreve 


    printf ("\n\n\nAndré Silva - A87958 , João Nunes - A87972\n");
    printf ("MIEI/CD, 2020\n");
    printf ("Módulo B: Cálculo das tabelas de codificação\n");
    printf ("Ficheiro: %s \n", fileName);
    printf ("Número de blocos: %llu\n", n_blocks);
    printf ("Tamanho dos blocos analisados no ficheiro de frequências: %lld blocos de %lu bytes + 1 bloco de %d bytes\n", n_blocks-1, block_size, size_of_last_block);
    printf ("Tamanho total do ficheiro: %llu\n", total);
    printf ("Tempo de execução: \n");
    printf ("Ficheiro gerado: \n");

}

void readFreqs (FILE *freq, int *indexPointer, char *buffer, long long block_size){

    fread(buffer, sizeof(char), block_size, freq);
}

int distArroba (FILE *freq, int size, int x){
    int i = 0;
    char c;
    int arrobas = 0;
    
    if (!x){
        fseek(freq, 256, SEEK_CUR);
        c = fgetc(freq);
        while (c != '@'){
            i++;
            c = fgetc(freq);
        }
    }
    else{
        fseek(freq, size, SEEK_CUR);
        c = fgetc(freq);
        while (c != '@'){
            i++;
            c = fgetc(freq);
        }
    }

    return i;
}

void toStruct (struct node nodes[],char *buffer, long long n_blocos, int *freq_array, int pos_freqs){
    int i = 0;
    int j = pos_freqs;
    char c;
    char str[20];

    while(j<strlen(buffer)){
        if (buffer[j] != ';'){
            if (buffer[j]!= '0'){
                nodes[i].init_symbol = j;
                while (buffer[j] != ';'){
                    c = buffer[j];
                    strncat(str,&c,1);
                    j++;
                }
                nodes[i].final_symbol = j-1;
                nodes[i].freq = atoi(str);
                if(atoi(str)!= 0) freq_array[i] = atoi(str);
                str[0] = '\0';
                i+=1;
            }
        }
        j++;
    }
}

void minSort (int *freqArray){
    int i,j,temp;

    for (i = 0; i < 256; i+=1){
        for (j = i + 1; j < 256; j++){
            if(freqArray[i] < freqArray[j]){
                temp = freqArray[i];
                freqArray[i] = freqArray[j];
                freqArray[j] = temp;
            }
        }
    }
}

int calcular_melhor_divisao (int *freqArray, int i, int j){
    int div=i, g1=0,total,mindif,dif;
    total=mindif=dif=soma(freqArray,i,j);
    printf("TOTAL-%d\n",total);
    do{
        printf("1 VEZ - %s\n", "...");
        g1=g1+freqArray[div];
        printf ("G1 - %d\n", g1);
        dif=abs(2*g1-total);
        printf ("DIF - %d\n", dif);
        if (dif<mindif){ 
            div=div+1;
            mindif=dif;
        }
        else dif=mindif+1;
    } while (dif!=mindif);
    return div-1;
 }

void calcular_codigos_SF (int *freqArray, char *codes, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col){

    if (start!=end){

        printf("%s\n","SF");
        int div=calcular_melhor_divisao(freqArray, start, end);
        printf ("DIV-%d\n",div);
        add_bit_to_code('0', codes, _matrix, start, div, col);
        add_bit_to_code('1', codes, _matrix, div+1, end, col);
        if(start==0){
            int i,j;
            char temp;
            for(j=1;j<strlen(codes); j+=2){
                for(i=strlen(codes); i>=j; i--){
                    codes[i]=codes[i-1];
                }
                codes[j-1] = ';';
            }
        }
        printf("\nCODES - %s\n", codes);
        col++;
        calcular_codigos_SF(freqArray, codes, _matrix, start, div, col);
        calcular_codigos_SF(freqArray, codes, _matrix, div+1, end, col);
    }
 } 

 int soma (int *freqArray, int i, int j){
     int total=0;

     for (i;i<=j;i+=1){
         if(freqArray[i]==0) break;
         printf("FRQ-%d\n", freqArray[i]);
         total += freqArray[i];
     }

     return total;
 }

 void add_bit_to_code (char c, char *codes, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col){
     
     printf("start = %d\n",start);
     printf("end = %d\n",end);
     printf("col = %d\n",col);
     printf("C = %c\n",c);

     for(start; start<=end;start++){
         _matrix[start][col] = c;
         printf("CHAR-%c\n",_matrix[start][col]);
     }

 }

/*
void add_bit_to_code(char c, char *codes, int start, int end) {
  char nCodes[end-start+1], *lCodes, *rCodes;
  printf("START %d, END %d\n", start, end);
  for (int i=0; i<sizeof(nCodes); i++){
    nCodes[i] = c;
  }
  printf("NCODES - %s\n",nCodes);

  int len = strlen(codes);
  lCodes = add_bit_to_code_AUX(codes, 0, start);
  printf("LCODES - %s\n",lCodes);
  rCodes = add_bit_to_code_AUX(codes, start, len-start+1);
  printf("RCODES - %s\n",rCodes);
 
  strcpy(codes, "");
  strcat(codes, lCodes);
  free(lCodes);

  strcat(codes, nCodes);
  strcat(codes, rCodes);
  free(rCodes);
}

char *add_bit_to_code_AUX(char *codes, int start, int len) {
  char *str;
  int c;

  str = malloc(len+1);
  for(c = 0; c<len; c++)
    *(str+c) = *((codes+start)+c);

  *(str+c) = '\0';
  return str;
}*/

        /*
     for (start; start<=end; start+=1){
        char *buf = (char *) malloc(strlen(codes)+3);
        strncpy (buf, codes, start);
        int len = strlen(buf);
        buf[];
     }*/
 




