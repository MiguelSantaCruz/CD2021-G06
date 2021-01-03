/* Autores: João Gonçalves - A93204 , Maria Gomes - A93314
Data: 03/01/2020

Na realização deste módulo começamos por ler os argumentos e passa-los à função ler_ficheiro.
A função ler_ficheiro com a utilização da função f_size atribui os valores à váriaveis 
n_blocks e size_of_last_block depois a estrutura de dados que contém toda a informção relativa a este 
módulo é inicializada. A leitura do ficheiro é efetuada por blocos de tamanho definido pelo utilizador
ou por defeito,utilizando a função fread, depois é chamada a função rle, que aplica o mecânismo de 
compressão RLE e devolve o tamanho do bloco resultante.
Dependendo do resultado da função taxaCompressao ou da opção do utilizador é ou não aplicado a todo o
ficheiro a compressão RLE, sendo aplicado o ficheiro .rle será escrito usando a função fwrite.
As frequencias de cada bloco são escrita pela função freq e guardadas na estrutura de dados.
Por fim, o ficheiro .freq será escrito usando a função escreve_freq ultizando a estrutura de dados.    
*/

#include "modulo_a.h"

/*Função que escreve o ficheiro com as frequencias, recebe uma estrura de dados com a informação 
necessária para escrever o ficheiro .freq e nome do ficheiro */

void escreve_freq (Stack *s, char a []){
    FILE *f;
    char or [] = ".freq";// serve para alterar o nome
    strcat(a,or);
    strcpy(s->nfreq,a);
    f = fopen (a,"w");

    if (s->rle) fprintf (f,"@R");
    else fprintf (f,"@N");

    fprintf (f,"@%d",s->n_blocos); // escreve o número de blocos

    for (int r = 0; r < s->n_blocos -1; r++){
        if (s->rle == 1) fprintf (f,"@%d@",s->tamBrle[r]); // escreve o tamanho de cada bloco
        else fprintf (f,"@%d@",s->tamB);

        fprintf (f,"%d;", (s->tab)[r].frequencia[0]); // escreve a frequencia do simbolo 0

        for(int i=1;i<255;i++) {
            int ant = (s->tab)[r].frequencia[i-1]; // comparação entre as frequencias
            int prox = (s->tab)[r].frequencia[i];
            if (prox == ant )fprintf (f,";");
            else fprintf (f,"%d;", (s->tab)[r].frequencia[i]);
    
        }
    }
    if (s->rle == 1) fprintf (f,"@%d@",s->tamBrle[(s->n_blocos) - 1]);
    else fprintf (f,"@%d@",s->tamU);
    
    fprintf (f,"%d;", (s->tab)[s->n_blocos-1].frequencia[0]); // escreve o último bloco
    for(int i=1;i<255;i++) {
        int ant = (s->tab)[s->n_blocos-1].frequencia[i-1];
        int prox = (s->tab)[s->n_blocos-1].frequencia[i];
        if (prox == ant )fprintf (f,";");
        else fprintf (f,"%d;",(s->tab)[s->n_blocos-1].frequencia[i]);
    }
    fprintf (f,"@0");
}


// Função para contabilizar a frequencia de cada carater no bloco
void freq (unsigned char *v, unsigned long tamBloco, int b, Stack *s){  
    for (int i = 0; i< 256; i++) s->tab[b].frequencia[i] = 0;
    int p;
    for(p = 0; p < tamBloco;p++){
        s->tab[b].frequencia[v[p]] += 1; 
    }  
}



// Função que efetua o mecanismo de RLE 
unsigned long rle (unsigned char * v, unsigned char * f,unsigned long tamBloco){
    int contaR = 1;
    int iv, i = 0;
    unsigned long t = tamBloco;
    for (iv = 0; iv < tamBloco; iv++){
        if (v[iv] == 0){                   // exceção para o carater 0
            f[i] = v[iv];
            f[i+1] = v[iv];
            sprintf(&f[i+2], "%c", contaR);
            i = i + 3;
            t += 2;

        }
        else {                                  // para qualquer carater que se repita mais de 3
            for (int j = iv + 1; j < tamBloco; j++){
                if (v[iv] == v[j]) contaR++;
                else j = tamBloco;    
            }
            if (contaR >= 4){
                f[i+1] = v[iv];
                f[i] = '\0';
                sprintf(&f[i+2], "%c", contaR);
                i = i + 3;
                iv += (contaR-1);
                t = t - (contaR-3);
            }
            else {
                f[i] = v[iv];
                i++;
            } 
        }
        contaR = 1;
    }
    return t;
}




// Função que indica a rentabilidade do mecanismo de compressão RLE
int taxaCompressao (unsigned long tam_I,unsigned long tam_F){
    int r = (tam_I - tam_F)*100;
    r = r/tam_I;
    return r;
}


// Função que ler o ficheiro 
int ler_ficheiro (char fic [],unsigned long tam_b, char a [], Stack *s,int r){
    FILE *f = fopen(fic,"rb");
    long long n_blocks;
    unsigned long size_of_last_block, block_size = tam_b;
    
    n_blocks = fsize(f, NULL, &block_size, &size_of_last_block);
    
    if (n_blocks != -3){               //controlo do tamanho do ficheiro e do último bloco
        
        FILE *fp = fopen(fic,"rb"); 
    
        s->tab = calloc(n_blocks, sizeof(struct freq_bloco)); // incialização da estrutura de dados
        s->rle = 0;
        s->n_blocos = n_blocks;
        s->tamU = size_of_last_block;
        if (n_blocks == 1){
            s->tamB = block_size = size_of_last_block;
            size_of_last_block = 0;
            s->tamU=0;
        }else{
            s->tamB = block_size;
            s->tamU = size_of_last_block;
        }
        strcpy (s->nome,"\0"); 
        
        unsigned char * buffer;
        unsigned char * buffer1;
        buffer = malloc(sizeof(unsigned char)*block_size);
        buffer1 = malloc(sizeof(unsigned char)*block_size * 2);
        
        fread(buffer,sizeof(unsigned char),block_size,fp);// le só o primeiro bloco
        
        int rl = rle(buffer,buffer1, block_size);
        
        int taxa = taxaCompressao(block_size, rl);
        s->taxaC = taxa;
        
        int i;
        if ((taxa < 5) && r == 0){ // não faz o rle
            freq(buffer,block_size,0,s);
            for (i = 1; i<= n_blocks - 2; i++){
                fread(buffer,sizeof(unsigned char),block_size,fp);
                freq(buffer,tam_b,i,s);
                
            }
        
            if (size_of_last_block != tam_b){
                fread(buffer,sizeof(unsigned char),size_of_last_block,fp);
                freq(buffer,size_of_last_block,i,s);
            }
            else {
                fread(buffer,sizeof(unsigned char),block_size,fp);
                freq(buffer,block_size,i,s);
            }  
        }
        else {
            
            freq(buffer1,rl,0,s);
            s->tamBrle[0] = rl;
            char or [] = ".rle";
            strcat(a,or);
            strcpy(s->nome,a);
            FILE *fw = fopen (a,"wb");
            s->rle = 1;
            fwrite(buffer1,sizeof(unsigned char),rl,fw);//escreve o .rle
            
           for (i = 1; i <= n_blocks - 2; i++){
                fread(buffer,sizeof(unsigned char),block_size,fp);
                int t =rle(buffer,buffer1, block_size);freq(buffer1,t,i,s);
                s->tamBrle[i] = t;
                fwrite(buffer1,sizeof(unsigned char),t,fw); 
            }
            if ((size_of_last_block != block_size) && size_of_last_block != 0){
                fread(buffer,sizeof(unsigned char),size_of_last_block,fp);
                int t = rle(buffer,buffer1, size_of_last_block);freq(buffer1,t,i,s);
                s->tamBrle[n_blocks-1] = t;
                fwrite(buffer1,sizeof(unsigned char),t,fw); 
            }
            else {
                if (size_of_last_block != 0){
                    fread(buffer,sizeof(unsigned char),block_size,fp);
                    int t = rle(buffer,buffer1, block_size);freq(buffer1,t,i,s);
                    s->tamBrle[n_blocks-1] = t;
                    fwrite(buffer1,sizeof(unsigned char),t,fw); 
                }
            }
        }  
        escreve_freq(s,a);
        free(buffer);
    }
    else {
        printf("Erro tamanho minimo do ficheiro não atingido\n\n");
        return -3;
    }
    return 1;
}
