#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulo_b.h"
#include <time.h>

#define NUMBER_OF_FREQ 256
//TODO 
//Verificar função stor
//; a mais
//@N@1@55@0;;;;;;10;9;9;9;9;9;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;0;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;@0
int main (int argc, char* argv[]){

    clock_t start_time = clock();

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
    char n_blocks_s[5];
    n_blocks_s[0]='\0';
    int arrobas = 0;
    int rOrN = 0; // N = 1 R = 0
    char str[12];
    char str_aux[12];
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
        } else if (arrobas == 2){
            int count=0;
            while(c !='@'){
                str_aux[count] = c;
                count++;
                c = fgetc(ficheiroFreq);
            }
            str_aux[count]='\0';
            strcat(n_blocks_s,str_aux);
            arrobas++;
        }

        if (arrobas == 4){
            break;
        }
    }

    int nb=atoi(n_blocks_s);
    n_blocks = nb;
    int pos_freqs = index-2;
    int posmove = index +1;
    index = index-k-1;
    fseek(ficheiroFreq, index, SEEK_SET);

    block_size = atoi(c_block_size);
    free(c_block_size);

    //Função que le
    
    int i = 0;
    int m = 1;
    int dist = 0;
    int indexpointer = index;
    char temp;
    char strtemp[12];
    struct node nodes[256];
    int array_size = 0;
    char *size_of_last_block_string;
    int pos = 0;
    char strtemp2[100];
    int fst_time = 0;
    char *exit_buffer;
    exit_buffer = (char *)malloc(256);
    char *codes;
    codes = (char *)malloc(256*256);
    int cont;

    FILE * ficheiroCod;
    char exit_file_name[16];
    if (rOrN == 1){
        ficheiroCod = fopen ("aaa.txt.cod", "wb");
        strcpy(exit_file_name, "aaa.txt.cod");
    }
    else{
        ficheiroCod = fopen ("aaa.txt.rle.cod", "wb");
        strcpy(exit_file_name, "aaa.txt.rle.cod");
    }
    
    if (ficheiroCod == NULL){
        printf ("ERRO: Erro ao abrir o ficheiro!\n");
        exit(3);
    }

    while (i<n_blocks-1){
        exit_buffer[0]='\0';
        codes[0] = '\0';
        char _matrix [NUMBER_OF_FREQ][NUMBER_OF_FREQ] = {'x'};
        array_size = 0;
        int freq_Array[256];
        memset(freq_Array,0,256);
        memset (&nodes, -1 ,256*sizeof(node));
        char *entry_buffer;
        dist = 256+distArroba (ficheiroFreq, 0, 0);
        entry_buffer = (char *)malloc(dist+1);
        fseek(ficheiroFreq, indexpointer, SEEK_SET);
        fread(entry_buffer, sizeof(char), dist, ficheiroFreq);
        //printf("ENTRYB-%s\n", entry_buffer);
        //printf("dist-%d, ind-%d\n", dist, indexpointer);
        //printf("posfreq-%d\n", pos_freqs);
        i++;
        indexpointer += dist;
        toStruct (nodes, entry_buffer, n_blocks, freq_Array, pos_freqs);

        for(int f=0; f<256; f++){
            if(freq_Array[f]!=0) array_size++;
        }

        minSort (freq_Array, array_size);

        // FIXED ATE AQUI!
        
        calcular_codigos_SF (freq_Array, _matrix, 0, array_size-1, 0);
        //printf("ERRO4 -\n");
        /*
        for(int f=0; f<256; f++){
            printf("ARR-%d\n",freq_Array[f]);
        }*/

        char d = ';';
        cont = 0;

        for(int i=0; i<NUMBER_OF_FREQ && i<array_size; i++){
            for(int j=0; j<NUMBER_OF_FREQ && j<array_size-1; j++){
                if(_matrix[i][j] =='0' || _matrix[i][j] =='1'){             
                    codes[cont]=_matrix[i][j];
                    cont++;
                    //printf("MATRIX- (%d,%d) - %c\n", i,j,_matrix[i][j]);
                    
                }
            }
            codes[cont]=d;
            cont++;
        }

        //printf("CODES- %s\n",codes);

        int f,h, g = -1;
        char tmp[256];

        for (int z = 0; z<strlen(codes); z++){
            g++;
            for (int k = 0; k<256; k++){
                //printf("STRLEN- %lu\n",strlen(nodes[k].code));
                if (nodes[k].freq==freq_Array[g] && (strcmp(nodes[k].code,"x")==0)){
                    f=z;
                    while (codes[f]!= ';'){
                        tmp[h]=codes[f];
                        h++;
                        f++;
                    }
                    z+=h;
                    tmp[h]='\0';
                    //printf("Strtemp - %s\n",tmp);
                    strcpy (nodes[k].code, tmp);
                    //printf("Nodes [%d] - %s\n",k,nodes[k].code);
                    h=0;
                    break;
                }
            }
        }
        

        if (fst_time == 0){
            strncat (exit_buffer, "@", 1);
            if(rOrN==1){
                strncat (exit_buffer, "N", 1);
            }
            else{strncat (exit_buffer, "R", 1);}
            strncat (exit_buffer, "@", 1);
            itoa(n_blocks,strtemp2);
            strcat (exit_buffer, strtemp2);
        }
        fst_time=1;
        strncat (exit_buffer, "@", 1);
        itoa(block_size,strtemp2);
        strcat (exit_buffer, strtemp2);
        strncat (exit_buffer, "@", 1);

        moveToBuffer(nodes, exit_buffer, pos, posmove);
        fwrite(exit_buffer, 1, strlen(exit_buffer), ficheiroCod);
    }


    array_size=0;
    codes[0]='\0';
    int freq_Array[256];
    memset(freq_Array,0,256);
    memset (&nodes, -1 ,256*sizeof(node));
    exit_buffer[0]='\0';
    char *entry_buffer;
    entry_buffer = (char *)malloc(dist+1);
    dist = 256+distArroba (ficheiroFreq, 0, 0);
    fseek(ficheiroFreq, indexpointer, SEEK_SET);
    char caux = fgetc(ficheiroFreq);
    int counter =0;
    char straux [12];
    
    
    while ((caux = fgetc(ficheiroFreq))!='@'){
        straux [counter] = caux;
        counter++;
    }
    straux[counter]='\0';
    size_of_last_block = atoi(straux);
    fseek(ficheiroFreq, indexpointer, SEEK_SET);
    fread(entry_buffer, sizeof(char), dist, ficheiroFreq);
    indexpointer += dist;
    toStruct (nodes, entry_buffer, n_blocks, freq_Array, pos_freqs);

    for(int f=0; f<256; f++){
            if(freq_Array[f]!=0) array_size++;
    }

    minSort (freq_Array, array_size);
        
    char _matrix [NUMBER_OF_FREQ][NUMBER_OF_FREQ] = {'x'};

    calcular_codigos_SF (freq_Array, _matrix, 0, array_size-1, 0);

    char d = ';';
    cont = 0;

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

    int f,h, g = -1;
    char tmp[256];

    for (int z = 0; z<strlen(codes); z++){
        g++;
        for (int k = 0; k<256; k++){
            //printf("STRLEN- %lu\n",strlen(nodes[k].code));
            if (nodes[k].freq==freq_Array[g] && (strcmp(nodes[k].code,"x")==0)){
                f=z;
                while (codes[f]!= ';'){
                    tmp[h]=codes[f];
                    h++;
                    f++;
                }
                z+=h;
                tmp[h]='\0';
                //printf("Strtemp - %s\n",tmp);
                strcpy (nodes[k].code, tmp);
                //printf("Nodes [%d] - %s\n",k,nodes[k].code);
                h=0;
                break;
            }
        }
    }
    exit_buffer[0]='\0';
    
    strncat (exit_buffer, "@", 1);
    if (n_blocks == 1){
        if(rOrN==1){
                strncat (exit_buffer, "N", 1);
            }
        else{
            strncat (exit_buffer, "R", 1);
            }
        strncat (exit_buffer, "@", 1);
        itoa(n_blocks,strtemp2);
        strcat (exit_buffer, strtemp2);
        strncat (exit_buffer, "@", 1);
        itoa(block_size, strtemp2);
    }
    else{
        itoa(size_of_last_block,strtemp2);
    }
    strcat(exit_buffer, strtemp2);
    strncat (exit_buffer, "@", 1);
    /*
    strcat (exit_buffer, strtemp2);
    printf ("ERRO1 \n");
    strcat (exit_buffer, strtemp2);
    strncat (exit_buffer, "@", 1);
    */
    printf ("EXT2 - %s\n", exit_buffer);
    if(n_blocks==1){
        moveToBuffer(nodes, exit_buffer, pos, posmove);
    }
    else{
        moveToBuffer(nodes, exit_buffer, pos, posmove);
    }
    strncat (exit_buffer, "@", 1);
    strncat (exit_buffer, "0", 1);
    printf ("EXT - %s\n", exit_buffer);
    fwrite(exit_buffer, 1, strlen(exit_buffer), ficheiroCod);
    printf ("ERRO2 \n");
    //free(exit_buffer);
    printf ("ERRO3 \n");
    free (entry_buffer);
    int last_block_size = total - ftell(ficheiroFreq)-1;

    fclose (ficheiroFreq);
    fclose (ficheiroCod);
    

    clock_t finish_time = clock();

    printf ("\n\n\nAndré Silva - A87958 , João Nunes - A87972\n");
    printf ("MIEI/CD, 2020\n");
    printf ("Módulo B: Cálculo das tabelas de codificação\n");
    printf ("Ficheiro: %s \n", fileName);
    printf ("Número de blocos: %llu\n", n_blocks);
    if (n_blocks == 1){
        printf ("Tamanho dos blocos analisados no ficheiro de frequências: %lld bloco de %lu bytes\n", n_blocks, block_size);
        printf ("Tamanho total do ficheiro: %llu\n", (n_blocks)*block_size);
    }
    else{
        printf ("Tamanho dos blocos analisados no ficheiro de frequências: %lld blocos de %lu bytes + 1 bloco de %d bytes\n", n_blocks-1, block_size, size_of_last_block);
        printf ("Tamanho total do ficheiro: %llu\n", (n_blocks-1)*block_size + size_of_last_block);
    }
    printf ("Tempo de execução: %f segundos\n", (double)(finish_time - start_time) / CLOCKS_PER_SEC);
    printf ("Ficheiro gerado: %s\n", exit_file_name);

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
    int counter=0;
    int counter2=0;
    //printf("Buf- %s",buffer);
    while(j<strlen(buffer)){
        if (buffer[j] != ';'){
            if(buffer[j]== '0'){
                counter++;
            }
            if (buffer[j]!= '0'){
                //printf("C1-%d C2-%d\n", counter, pos_freqs);
                nodes[i].init_symbol = j+3-counter;
                while (buffer[j] != ';'){
                    c = buffer[j];
                    strncat(str,&c,1);
                    j++;
                }
                strcpy(nodes[i].code, "x");
                nodes[i].final_symbol = j+2-counter;
                nodes[i].freq = atoi(str);
                if(atoi(str)!= 0) freq_array[i] = atoi(str);
                str[0] = '\0';
                i+=1;
            }
        }
        j++;
    }
}

void minSort (int *freqArray, int array_size){
    int i,j,temp;

    for (i = 0; i <array_size; i++){
        for (j = i + 1; j < array_size; j++){
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
    //printf("Total - %d\n", total);
    do{
        g1=g1+freqArray[div];
        dif=abs(2*g1-total);
        if (dif<mindif){ 
            div=div+1;
            mindif=dif;
        }
        else dif=mindif+1;
    } while (dif!=mindif);
    return div-1;
 }

void calcular_codigos_SF (int *freqArray, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col){
    //printf("ERRO3 -\n");
    if (start!=end){
        int div=calcular_melhor_divisao(freqArray, start, end);
        //printf ("DIV - %d\n", div);
        add_bit_to_code('0',_matrix, start, div, col);
        add_bit_to_code('1', _matrix, div+1, end, col);
        col++;
        calcular_codigos_SF(freqArray, _matrix, start, div, col);
        calcular_codigos_SF(freqArray, _matrix, div+1, end, col);
        //printf("ERRO -\n");
    }
    //printf("ERRO2 -\n");
 } 

 int soma (int *freqArray, int i, int j){
     int total=0;

     for (i;i<=j;i+=1){
         if(freqArray[i]==0) break;
         total += freqArray[i];
     }

     return total;
 }

 void add_bit_to_code (char c, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col){

     for(start; start<=end;start++){
         _matrix[start][col] = c;
     }

 }

 void itoa(int n, char s[]){
     int i, sign;

     if ((sign = n) < 0) 
         n = -n;         
     i = 0;
     do {      
         s[i++] = n % 10 + '0';  
     } while ((n /= 10) > 0);     
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

 void reverse(char s[]){
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 } 


 void moveToBuffer(struct node nodes[], char *exitBuffer, int pos, int posmove){
     char *c;
     int k=posmove;
     int counter=0;
     int counter2 = -1;
     printf("P-%d,PM-%d\n", pos, posmove);
     
     for (int i=0; i<256; i++){
         if (nodes[i].init_symbol != -1){
            printf("k,Node -%d, %d, %d\n", k,nodes[i].init_symbol,nodes[i].init_symbol+counter2);
            while(k<nodes[i].init_symbol+counter2){ 
                k++;
                strncat(exitBuffer, ";", 1);
                counter++;
            }
            printf("K-%d\n",k);
            //printf("EXTBUF - %s\n", exitBuffer);
            //printf("Nodes[%d] - %s\n", i, nodes[i].code);
            strcat(exitBuffer, nodes[i].code);
            k+=strlen(nodes[i].code);
            counter2+=strlen(nodes[i].code)-1;
            strncat(exitBuffer, ";", 1);
            counter++;
            k++;
            if ((nodes[i].final_symbol - nodes[i].init_symbol+1)>strlen(nodes[i].code)){
                printf("KK -%d, CDS -%lu\n",k,strlen(nodes[i].code));
                k+=nodes[i].final_symbol - nodes[i].init_symbol+3-strlen(nodes[i].code);
            }
            
         }
         else{
             while(counter<255){  
                strncat(exitBuffer, ";", 1);
                counter++;
             }
             break;
         }
     }
 }



