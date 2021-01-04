/*
Autores: João Nunes - A87972 , André Silva - A87958
Data: 01/01/2020
Versão: Final

Na realização deste trabalho começamos por identificar se os argumentos dados são válidos. De seguida,
abrimos o ficheiro dado e passamos para variaveis, informação que irá ser necessaria no seguimento do
programa. Após isto, copiamos do ficheiro para o buffer de entrada um bloco, contruimos uma estrutura de dados
com a posição de cada simbolo e a respetiva frequencia. Ao mesmo tempo, criamos um array de frequencias
ordenado decrescentemente. Por fim aplicamos o algoritmo de Shanon-Fano para criar os codigos de cada símbolo 
e guarda-los numa matriz. Apos isto é atualizada a nossa estrutura de dados para conter os codigos de cada
simbolo para, posteriormente, lermos da estrutura de dados e escrevermos no buffer de saida o bloco processado.
Depois de obtermos o bloco totalmente processado e escrito no buffer de saida, este será escrito no ficheiro
final.
Esta operação repete-se para todos os blocos exceto o ultimo. Este é tratado à parte por ter um tamanho
de bloco diferente.

Foi utilizada uma estrutura de dados que contém toda a informação necessaria para o processamento do bloco.
Esta está explicada em detalhe no ficheiro modulo_b.h.
Ao longo do código estão explicados os objetivos das variaveis mais importantes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "modulo_b.h"
#include <time.h>

#define NUMBER_OF_FREQ 256

void shanonFano (int argc, char *argv[]){

    clock_t start_time = clock(); //Iniciar contador do tempo de execução
    int verbose=0;

    if(argc!=4 && argc!=5){
        printf ("Número de argumentos incorreto.\nUsar: ./shafa exemplo.txt.freq -m t (-v)");
        exit(1);
    }

    if(strcmp(argv[2], "-m") || strcmp(argv[3], "t")){
        printf ("Argumentos incorretos.\nUsar: ./shafa exemplo.txt.freq -m t (-v)");
        exit(4);
    }

    if(argc==5){
        if(!strcmp(argv[4], "-v")){
            verbose = 1;
        }
    }

    int len=0;
    len = strlen(argv[1]+1);
    char fileName[len];
    strcpy(fileName,argv[1]);
    char codeFileName[len+1];
    int y;

    for(y=0; y<strlen(fileName)-4;y++){ //Guardar nome do ficheiro dado na string que tem o nome do ficheiro de saida
        codeFileName[y] = fileName[y];
    }
    codeFileName[y]='\0';

    FILE * ficheiroFreq; 
    ficheiroFreq = fopen (fileName, "rb"); //Abrir o ficheiro recebido
    if (ficheiroFreq == NULL){
        printf ("ERRO: Ficheiro não existe!\n");
        exit(2);
    }

    long long n_blocks; //numero de blocos
    unsigned long block_size; //tamanho do bloco
    int size_of_last_block; //tamanho do ultimo bloco

    fseek (ficheiroFreq, 0L, SEEK_SET);
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

    while ((c = fgetc(ficheiroFreq)) != EOF){ //Ciclo que determina se é normal ou rle, o numero de blocos e o tamanho dos blocos
        index++;
        if (c == 'N') rOrN = 1; //Guardar se o modo é normal ou RLE
        if (a == c) arrobas++;
        else if (arrobas == 3){
            if (k==1){
                c_block_size = malloc(k+1); 
                k++;
            }
            else {
                c_block_size = realloc(c_block_size, k+1); 
                k++;
            }
            sprintf(str,"%d", atoi(&c));
            strcat (c_block_size, str); //Guardar o tamanho do bloco
        } else if (arrobas == 2){
            int count=0;
            while(c !='@'){
                str_aux[count] = c;
                count++;
                c = fgetc(ficheiroFreq);
            }
            str_aux[count]='\0';
            strcat(n_blocks_s,str_aux); //Guardar o numero de blocos
            arrobas++;
            index+=count;
        }
        if (arrobas == 4){
            break;
        }
    }

    int n_blocks_aux = strlen(n_blocks_s);
    int nb=atoi(n_blocks_s);
    n_blocks = nb;
    int pos_freqs = index-3-strlen(n_blocks_s); //posição em que as frequencias se encontram no buffer
    int posmove = index;
    index = index-k-1;
    fseek(ficheiroFreq, index, SEEK_SET);

    block_size = atoi(c_block_size);
    
    int i = 0;
    int dist = 0;
    int indexpointer = index;
    struct node nodes[256]; //struct que guarda os dados de cada simbolo
    int array_size = 0; //tamanho do array das frequencias
    char strtemp2[100];
    int fst_time = 0; //Variavel que diz se estamos no primeiro bloco
    char *exit_buffer; //buffer de saida
    exit_buffer = (char *)malloc(256);
    char *codes; //variavel que guarda os codigos de todos os simbolos de um bloco ja separados por ;.
    codes = (char *)malloc(256*256);
    int cont;

    strcat(codeFileName, "cod");
    FILE * ficheiroCod;
    ficheiroCod = fopen (codeFileName, "wb"); //Abertura do ficheiro de saida
    
    if (ficheiroCod == NULL){
        printf ("ERRO: Erro ao abrir o ficheiro!\n");
        exit(3);
    }

    while (i<n_blocks-1){ //Ciclo que corre para os blocos que tem tamanho igual

        exit_buffer[0]='\0'; //Reset de varias variaveis usadas durante o ciclo
        codes[0] = '\0';
        char _matrix [NUMBER_OF_FREQ][NUMBER_OF_FREQ] = {'x'};
        array_size = 0;
        int freq_Array[256]; //Array das frequencias
        memset(freq_Array,0,256);
        memset (&nodes, -1 ,256*sizeof(node));
        char *entry_buffer;
        dist = 256+distArroba (ficheiroFreq); //Distancia ate ao fim do bloco
        entry_buffer = (char *)malloc(dist+1);
        fseek(ficheiroFreq, indexpointer, SEEK_SET);
        fread(entry_buffer, sizeof(char), dist, ficheiroFreq); //Leitura de um bloco do ficheiro para o buffer
        entry_buffer[dist]='\0';
        i++;
        indexpointer += dist;
        toStruct (nodes, entry_buffer, freq_Array, pos_freqs, n_blocks_aux); //Passar os dados do buffer para uma struct de modo a organizar os dados

        pos_freqs = strlen(c_block_size)+2;
        for(int f=0; f<256; f++){ //Calculo do tamanho do array de frequencias (numero de simbolos)
            if(freq_Array[f]!=0) array_size++;
        }

        arraySort (freq_Array, array_size); //Função que ordena com ordem decrescente as frequencias

        /*
        for(int h =0; h<array_size; h++){
            printf("Freq - %d \n",freq_Array[h]);
        }*/

        if(verbose){
            for(int h=0; h<array_size; h++){
                printf("Freq[%d] = %d\n", h, freq_Array[h]);
            }
            printf("\n");
        }
        
        calcular_codigos_SF (freq_Array, _matrix, 0, array_size-1, 0); //Função que calcula os codigos Shanon-Fano

        if(verbose){
            for(int i=0; i<NUMBER_OF_FREQ && i<array_size; i++){
                printf("Codigo[%d] - ", i);
                for(int j=0; j<NUMBER_OF_FREQ && j<array_size-1; j++){
                    printf("%c", _matrix[i][j]);
                }
                printf("\n");
            }
        }

        char d = ';';
        cont = 0;

        for(int i=0; i<NUMBER_OF_FREQ && i<array_size; i++){ //Ciclo que dada a matrix com os codigos passa esses codigos para a string codes
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

        for (int z = 0; z<strlen(codes); z++){ //Função que atualiza a struct para conter os codigos equivalentes a cada frequencia
            g++;
            for (int k = 0; k<256; k++){
                if (nodes[k].freq==freq_Array[g] && (strcmp(nodes[k].code,"x")==0)){
                    f=z;
                    while (codes[f]!= ';'){
                        tmp[h]=codes[f];
                        h++;
                        f++;
                    }
                    z+=h;
                    tmp[h]='\0';
                    strcpy (nodes[k].code, tmp);
                    h=0;
                    break;
                }
            }
        }
        
        if (fst_time == 0){ //Se for o primeiro bloco temos de acrescentar ao buffer de saida o tipo de ficheiro e o numero de blocos
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
        itoa(block_size,strtemp2); //Adicionar ao buffer de saida o tamaho do bloco lido
        strcat (exit_buffer, strtemp2);
        strncat (exit_buffer, "@", 1);

        moveToBuffer(nodes, exit_buffer, posmove); //Função que le os dados da struct e constroi o buffer de saida 
        fwrite(exit_buffer, 1, strlen(exit_buffer), ficheiroCod); //Função que escreve no ficheiro de saida os dados do buffer
        free(entry_buffer);
    }

    //Codigo referente ao ultimo bloco, ou primeiro caso so haja 1 bloco
    array_size=0; //Reset de varias variaveis usadas durante o ciclo
    codes[0]='\0';
    int freq_Array[256]; //Array das frequencias
    memset(freq_Array,0,256);
    memset (&nodes, -1 ,256*sizeof(node));
    exit_buffer[0]='\0';
    char *entry_buffer; //Buffer de entrada
    entry_buffer = (char *)malloc(dist+5);
    dist = 256+distArroba (ficheiroFreq); //Distancia ate ao fim do bloco
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
    pos_freqs = strlen(straux)+2; //posição das frequencias no array das frequencias
    fseek(ficheiroFreq, indexpointer, SEEK_SET);
    fread(entry_buffer, sizeof(char), dist, ficheiroFreq); //Função que le do ficheio para o buffer
    entry_buffer[dist]='\0';
    indexpointer += dist;
    toStruct (nodes, entry_buffer, freq_Array, pos_freqs, n_blocks_aux); //Passar os dados do buffer para uma struct de modo a organizar os dados

    for(int f=0; f<256; f++){ //Calculo do tamanho do array de frequencias (numero de simbolos)
            if(freq_Array[f]!=0) array_size++;
    }

    arraySort (freq_Array, array_size); //Função que ordena com ordem decrescente as frequencias

    if(verbose){
        for(int h=0; h<array_size; h++){
            printf("Freq[%d] = %d\n", h, freq_Array[h]);
        }
        printf("\n");
    }

    char _matrix [NUMBER_OF_FREQ][NUMBER_OF_FREQ] = {'x'};

    calcular_codigos_SF (freq_Array, _matrix, 0, array_size-1, 0); //Função que calcula os codigos Shanon-Fano

    if(verbose){
        for(int i=0; i<NUMBER_OF_FREQ && i<array_size; i++){
            printf("Codigo[%d] - ", i);
            for(int j=0; j<NUMBER_OF_FREQ && j<array_size-1; j++){
                printf("%c", _matrix[i][j]);
            }
            printf("\n");
        }
    }

    char d = ';';
    cont = 0;

    for(int i=0; i<NUMBER_OF_FREQ && i<array_size; i++){ //Ciclo que dada a matrix com os codigos passa esses codigos para a string codes
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

    for (int z = 0; z<strlen(codes); z++){ //Função que atualiza a struct para conter os codigos equivalentes a cada frequencia
        g++;
        for (int k = 0; k<256; k++){
            if (nodes[k].freq==freq_Array[g] && (strcmp(nodes[k].code,"x")==0)){
                f=z;
                while (codes[f]!= ';'){
                    tmp[h]=codes[f];
                    h++;
                    f++;
                }
                z+=h;
                tmp[h]='\0';
                strcpy (nodes[k].code, tmp);
                h=0;
                break;
            }
        }
    }
    exit_buffer[0]='\0';
    
    strncat (exit_buffer, "@", 1);
    if (n_blocks == 1){ //Se so ouver um bloco nao entrou no ciclo entao temos de adicionar ao buffer de saida 'N' ou 'R' referente a compressão Normal o RLE
        if(rOrN==1){
                strncat (exit_buffer, "N", 1);
            }
        else{
            strncat (exit_buffer, "R", 1);
            }
        strncat (exit_buffer, "@", 1);
        itoa(n_blocks,strtemp2);
        strcat (exit_buffer, strtemp2); //Adicionar ao buffer de saida que so foi lido um bloco
        strncat (exit_buffer, "@", 1);
        itoa(block_size, strtemp2); //Adicionar ao buffer de saida o tamaho do bloco lido caso haja so 1 bloco
    }
    else{
        itoa(size_of_last_block,strtemp2); //Adicionar ao buffer de saida o tamaho do ultimo bloco lido 
    }

    strcat(exit_buffer, strtemp2);
    strncat (exit_buffer, "@", 1);

    moveToBuffer(nodes, exit_buffer, posmove); //Função que le os dados da struct e constroi o buffer de saida 

    strncat (exit_buffer, "@", 1);
    strncat (exit_buffer, "0", 1); //AAdicionar '0' que significa fim do ficheiro
    fwrite(exit_buffer, 1, strlen(exit_buffer), ficheiroCod); //Função que escreve no ficheiro de saida os dados do buffer
    free (entry_buffer);
    free(c_block_size);

    fclose (ficheiroFreq);
    fclose (ficheiroCod);

    clock_t finish_time = clock(); //Guardar o tempo final

    //Prints finais
    printf ("\n\nAndré Silva - A87958 , João Nunes - A87972\n");
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
    printf ("Ficheiro gerado: %s\n\n\n", codeFileName);
}

int distArroba (FILE *freq){ //Função que calcula a distancia ate ao fim do bloco
    int i=0;
    char c;
    
    fseek(freq, 256, SEEK_CUR);
    c = fgetc(freq);
    while (c != '@'){
        i++;
        c = fgetc(freq);
    }
    return i;
}

void toStruct (struct node nodes[],char *buffer, int *freq_array, int pos_freqs, int block_size){ //Função que constroi a struct com os dados do buffer e o array das frequencias
    int i = 0;
    int j = pos_freqs;
    char c;
    char str[20];
    int counter=0;

    while(j<strlen(buffer)){
        if (buffer[j] != ';'){
            if(buffer[j]== '0'){
                counter++;
            }
            if (buffer[j]!= '0'){
                nodes[i].init_symbol = j+3-counter+block_size; //Guardar a posição inicial da frequencia
                while (buffer[j] != ';' && j<strlen(buffer)){ //Percorrer a frequencia para chegar á posição final
                    c = buffer[j];
                    strncat(str,&c,1); //Concatenar na variavel str a frequencia
                    j++;
                }
                strcpy(nodes[i].code, "x");
                nodes[i].final_symbol = j+2-counter+block_size; //Guardar a posição final
                nodes[i].freq = atoi(str); //Guardar a frequencia na struct
                if(atoi(str)!= 0) freq_array[i] = atoi(str); //Guardar a frequencia no array das frequencias
                str[0] = '\0'; //Reset da variavel str
                i+=1;
            }
        }
        j++;
    }
}

void arraySort (int *freqArray, int array_size){ //Função que ordena o array das frequencias
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

int calcular_melhor_divisao (int *freqArray, int i, int j){ //Funçao que calcula a melhor divisão para calcular os codigos
    int div=i, g1=0,total,mindif,dif;
    total=mindif=dif=soma(freqArray,i,j);

    do{
        g1=g1+freqArray[div];
        dif=abs(2*g1-total);
        if (dif<mindif){ 
            div=div+1;
            mindif=dif;
        }
        else dif=mindif+1;
    } while (dif==mindif);
    return div-1;
 }

void calcular_codigos_SF (int *freqArray, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col){ //Função que calcula os codigos e os guarda numa matriz

    if (start!=end){
        int div=calcular_melhor_divisao(freqArray, start, end);
        add_bit_to_code('0',_matrix, start, div, col);
        add_bit_to_code('1', _matrix, div+1, end, col);
        col++;
        calcular_codigos_SF(freqArray, _matrix, start, div, col);
        calcular_codigos_SF(freqArray, _matrix, div+1, end, col);
    }
 } 

 int soma (int *freqArray, int i, int j){ //Função que soma os elementos do array entre fuas posições
     int total=0;

     for (i;i<=j;i+=1){
         if(freqArray[i]==0) break;
         total += freqArray[i];
     }

     return total;
 }

 void add_bit_to_code (char c, char _matrix[NUMBER_OF_FREQ][NUMBER_OF_FREQ], int start, int end, int col){ //Função que adiciona '0' ou '1' ao respetivo codigo

     for(start; start<=end;start++){
         _matrix[start][col] = c;
     }
 }

 void itoa(int n, char s[]){ //Função de conversão de int para string
     int i=0, n_aux;

     if ((n_aux = n) < 0){
         n = -n;
     } 

     do {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);

     if (n_aux < 0){
         s[i++] = '-';
     }

     s[i] = '\0';
     reverse(s);
 }

 void reverse(char s[]){ //Função auxiliar da itoa que inverte uma string
     int i, j;
     char aux;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--){
         aux = s[i];
         s[i] = s[j];
         s[j] = aux;
     }
 } 

 void moveToBuffer(struct node nodes[], char *exitBuffer, int posmove){ //Função que dada a strut com os dados move esses dados para o buffer de saida
     int k=posmove;
     int counter=0;
     int counter2 = 0;
     
     for (int i=0; i<256; i++){
         if (nodes[i].init_symbol != -1){
            while(k<nodes[i].init_symbol+counter2){ //Escrever ";" até chegarmos ao local onde temos de escrever o codigo do simbolo
                k++;
                strncat(exitBuffer, ";", 1);
                counter++;
            }
            strcat(exitBuffer, nodes[i].code); //Escrever o codigo do simbolo
            k+=strlen(nodes[i].code);
            counter2+=strlen(nodes[i].code)-(nodes[i].final_symbol - nodes[i].init_symbol+1);
            if(counter!=255){
                counter++;
                k++;
                strncat(exitBuffer, ";", 1); //Adicionar ";" depois de cada codigo
            }
            if ((nodes[i].final_symbol - nodes[i].init_symbol+1)>strlen(nodes[i].code)){ 
                k+=nodes[i].final_symbol - nodes[i].init_symbol+1-strlen(nodes[i].code);
            }//Se o comprimento da frequencia for maior que o comprimento do codigo essa diferença é incrementada a k
         }
         else{
             while(counter<255){//Adicionar os ";" restantes
                strncat(exitBuffer, ";", 1);
                counter++;
             }
             break;
         }
     }
 }
 