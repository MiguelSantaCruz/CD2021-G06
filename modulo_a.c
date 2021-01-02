#include "modulo_a.h"

// Função que escreve o ficheiro com as frequencias, recebe uma estrura de dados com informação necessárias
// para escrever o ficheiro e nome do ficheiro 
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


// Função para contabilizar a frequencia de cada carater no bloco b e retorna a frequencia do carater 0,
// útil para o RLE
void freq (unsigned char *v, unsigned long tamBloco, int b, Stack *s){  
    for (int i = 0; i< 256; i++) s->tab[b].frequencia[i] = 0;
    int p;
    for(p = 0; p < tamBloco;p++){
        s->tab[b].frequencia[v[p]] += 1; 
    }  
}



// Função que efetua o mecanismo de RLE 
unsigned long rle (unsigned char * v, unsigned long tamBloco){
    int contaR = 1;
    
    for (int i = 0; i < tamBloco; i++){
        if (v[i] == 0){                   // exceção para o carater 0
            int ultposicao = tamBloco-1;
            for(int g = tamBloco+1 ; tamBloco-i < g; g--) {
                v[g] = v[ultposicao--];
            }
            v[i+1] = v[i]; //v[i] = '\0';
            sprintf(&v[i+2], "%c", contaR);
            i = i + 2;
            tamBloco = tamBloco + 2;
        }
        else {                                  // para qualquer carater que se repita mais de 3
            for (int j = i+1; j < tamBloco; j++){
                if (v[i] == v[j]) contaR++;
                else j = tamBloco;    
            }
            if (contaR >= 4){
                v[i+1] = v[i];
                v[i] = '\0';
                sprintf(&v[i+2], "%c", contaR);
                for(int a = i+3; a < tamBloco; a++) {
                    v[a] = v[a + (contaR-3)];
                }
                i = i + 2;
                tamBloco = tamBloco - (contaR-3);
            }
        }
        contaR = 1;
    }
    return tamBloco;
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
    unsigned long long total;
    long long n_blocks;
    unsigned long size_of_last_block, block_size = tam_b;
    
    n_blocks = fsize(f, NULL, &block_size, &size_of_last_block);
    
    if (n_blocks != -3){               //controlo do tamanho do ficheiro e do último bloco
        int n = 0;
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
        buffer = malloc(sizeof(unsigned char)*block_size * 2);
        
        n = fread(buffer,sizeof(unsigned char),block_size,fp);// le só o primeiro bloco
        freq(buffer,block_size,0,s);
        
        int rl = rle(buffer,block_size);

        int taxa = 2;//taxaCompressao(block_size, rl);
        s->taxaC = taxa;
        
        int i;
        if ((taxa >= 0 && taxa < 5) && r == 0){ // não faz o rle
            
            for (i = 1; i<= n_blocks - 2; i++){
                fread(buffer,sizeof(unsigned char),block_size,fp);
                freq(buffer,tam_b,i,s);
                
            }
        
            if (size_of_last_block != tam_b){
                n = fread(buffer,sizeof(unsigned char),size_of_last_block,fp);
                freq(buffer,size_of_last_block,i,s);
            }
            else {
                fread(buffer,sizeof(unsigned char),block_size,fp);
                freq(buffer,block_size,i,s);
            }  
        }
        else {
            
            freq(buffer,rl,0,s);
            s->tamBrle[0] = rl;
            char or [] = ".rle";
            strcat(a,or);
            strcpy(s->nome,a);
            FILE *fw = fopen (a,"wb");
            s->rle = 1;
            fwrite(buffer,sizeof(unsigned char),rl,fw);//escreve o .rle
            
           for (i = 1; i <= n_blocks - 2; i++){
                fread(buffer,sizeof(unsigned char),block_size,fp);
                int t =rle(buffer,block_size);freq(buffer,t,i,s);
                s->tamBrle[i] = t;
                fwrite(buffer,sizeof(unsigned char),t,fw); 
            }
            if ((size_of_last_block != block_size) && size_of_last_block != 0){
                fread(buffer,sizeof(unsigned char),size_of_last_block,fp);
                int t = rle(buffer,size_of_last_block);freq(buffer,t,i,s);
                s->tamBrle[n_blocks-1] = t;
                fwrite(buffer,sizeof(unsigned char),t,fw); 
            }
            else {
                if (size_of_last_block != 0){
                    fread(buffer,sizeof(unsigned char),block_size,fp);
                    int t = rle(buffer,block_size);freq(buffer,t,i,s);
                    s->tamBrle[n_blocks-1] = t;
                    fwrite(buffer,sizeof(unsigned char),t,fw); 
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



//codigo interpretador



void main (int argc, char *argv[]) {
    clock_t begin = clock(),final;
    //int count;

    Stack s;
    int tam = 65536;
    int p = strlen(argv[1]);
    char a [p + 30];
    strcpy(a,argv[1]);

    if (//strcmp (argv[0], "Shafa") == 0 &&
        strcmp (argv[2], "-m") == 0 &&
        strcmp (argv[3], "f") == 0){
            int r = 0;
            if (argc > 4){
                for (int d=4; d<argc; d++){
                    if (strcmp (argv[d], "-c") == 0) r = 1;
                    if (strcmp (argv[d], "-b") == 0) tam = atoi (argv[d+1]);
                }
            }
            
            int c = ler_ficheiro (argv[1],tam, a,&s,r);           
            
            clock_t end = clock();
            final = ((double)(end - begin))*1000/ CLOCKS_PER_SEC;

            printf ("José Gonçalves, Maria Gomes, MIEI/CD, 1-jan-2021\nModulo: f (cálculo das frequências dos símbolos)\n");
            if (c == 1){
                printf ("Número de Blocos : %d\n", s.n_blocos);                
                printf ("Tamanho dos blocos analisados no ficheiro Original: %d/%d bytes\n",s.tamB,s.tamU);
                if (r == 1 || s.rle == 1) {
                    printf ("Compressão RLE: %s (%d  compressão)\n",s.nome, s.taxaC);
                    printf ("Tamanho dos blocos analisados no ficheiro RLE: %d/%d bytes\n", s.tamBrle[0],
                                    ((s.n_blocos == 1 ) ? (s.tamBrle[s.n_blocos]) : (s.tamBrle[s.n_blocos -1])));
                }
                printf ("Tempo de execução do módulo (milissegundos): %ld\n", final);
        
                printf ("Ficheiros gerados: %s %s\n",s.nome, s.nfreq); 
            
            }
        }
    else printf("Não é este módulo\n");
}
