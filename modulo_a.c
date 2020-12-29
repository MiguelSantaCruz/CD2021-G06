#include "modulo_a.h"

void escreve_freq (Stack *s, char a []){
    FILE *f;
    char or [] = ".freq";
    strcat(a,or);
    strcpy(s->nfreq,a);
    f = fopen (a,"w");

    if (s->rle) fprintf (f,"@R");
    else fprintf (f,"@N");

    fprintf (f,"@%d",s->n_blocos);

    for (int r = 0; r < s->n_blocos -1; r++){
        if (s->rle == 1) fprintf (f,"@%d@",s->tamBrle[r]);
        else fprintf (f,"@%d@",s->tamB);

        fprintf (f,"%d;", (s->tab)[r].frequencia[0]);

        for(int i=1;i<255;i++) {
            int ant = (s->tab)[r].frequencia[i-1];
            int prox = (s->tab)[r].frequencia[i];
            if (prox == ant )fprintf (f,";");
            else fprintf (f,"%d;", (s->tab)[r].frequencia[i]);
    
        }
    }
    if (s->rle == 1) fprintf (f,"@%d@",s->tamBrle[(s->n_blocos) - 1]);
    else fprintf (f,"@%d@",s->tamU);
    
    fprintf (f,"%d;", (s->tab)[s->n_blocos-1].frequencia[0]);

    for(int i=1;i<255;i++) {
        int ant = (s->tab)[s->n_blocos-1].frequencia[i-1];
        int prox = (s->tab)[s->n_blocos-1].frequencia[i];
        if (prox == ant )fprintf (f,";");
        else fprintf (f,"%d;",(s->tab)[s->n_blocos-1].frequencia[i]);
    }
    fprintf (f,"@0");
}



int freq (unsigned char v [], int n, int pB, Stack *s){
    int i, freq = 0,nNull = 0;
    int fq[256];
    
    for (i = 0; i < 255; i++){
        
        for(int p = 0; p < n; p++){
            if (v[p] == i) freq++;   
        }
        s->tab[pB].frequencia[i] = freq;
        freq = 0;
    }
    return nNull = s->tab[pB].frequencia[0]; 
}


int rle (unsigned char v [], int n,int pB, Stack *s){
    int contaR = 1;
    
    for (int i = 0; i < n; i++){
        if (v[i] == '\0'){
            int ultp = n-1;
            for(int g = n+1 ; n-i < g; g--) {
                v[g] = v[ultp--];
            }
            v[i+1] = v[i];
            //v[i] = '\0';
            sprintf(&v[i+2], "%c", contaR);
            i = i + 2;
            n = n + 2;
        }
        else {
            for (int j = i+1; j < n; j++){
                if (v[i] == v[j]) contaR++;
                else j = n;    
            }
            if (contaR >= 4){
                v[i+1] = v[i];
                v[i] = '\0';
                sprintf(&v[i+2], "%c", contaR);
                for(int a = i+3; a < n; a++) {
                    v[a] = v[a + (contaR-3)];
                }
                i = i + 2;
                n = n - (contaR-3);
            }
        }
        contaR = 1;
    }
    int freqN = 0;
    for(int p = 0; p < n; p++){//conta o carater null
            if (v[p] == '\0') freqN++;
    }
    s->tab[pB].frequencia[0] += freqN;// fq[i] = freq;
    return n;
}


int taxaCompressao (int tam_I,int tam_F){
    int r = (tam_I - tam_F)*100;
    r = r/tam_I;
    return r;
}


int ler_ficheiro (char fic [],unsigned long tam_b, char a [], Stack *s,int r){
    FILE *f = fopen(fic,"rb");
    unsigned long long total;
    long long n_blocks;
    unsigned long size_of_last_block, block_size = tam_b;
    
    n_blocks = fsize(f, NULL, &block_size, &size_of_last_block);
    //controlo do tamanho do último bloco
    if (n_blocks != -3){
        int n = 0;
        FILE *fp = fopen(fic,"rb");
    
        unsigned char v [block_size*2]; 
    
    
        s->tab = calloc(n_blocks, sizeof(struct freq_b));
        s->rle = 0;
        s->n_blocos = n_blocks;
        s->tamU = size_of_last_block;
        if (n_blocks ==1){
            s->tamB = block_size = size_of_last_block;
            s->tamU=0;
        }else{
            s->tamB = block_size;
            s->tamU = size_of_last_block;
        }
        strcpy (s->nome,"\0"); 
    
        n = fread( &v,sizeof(char),block_size,fp);// le só o primeiro bloco
        int z = freq(v,block_size,0,s);

        int rl = rle(v,block_size,0,s);
    
        int taxa = taxaCompressao(block_size, rl);
        s->taxaC = taxa; 
        int i;
        if ((taxa >= 0 && taxa < 5) && r == 0){ // não faz o rle
            s->tab[0].frequencia[0] = z;
            for (i = 1; i<= n_blocks - 2; i++){
                n = fread(&v,sizeof(char),block_size,fp);
                freq(v,tam_b,i,s);
            }
        
            if (size_of_last_block != tam_b){
                n = fread( &v,sizeof(char),size_of_last_block,fp);
                freq(v,size_of_last_block,i,s);
            }
            else {
                fread(&v,sizeof(char),block_size,fp);
                freq(v,block_size,i,s);
            }  
        }
        else {
            s->tamBrle[0] = rl;
        
            char or [] = ".rle";
            strcat(a,or);
            strcpy(s->nome,a);
            FILE *fw = fopen (a,"wb");
            s->rle = 1;
            fwrite(&v,sizeof(char),rl,fw);//escreve o .rle

           for (i = 1; i<= n_blocks - 2; i++){
                n = fread( &v,sizeof(char),block_size,fp);
                freq(v,block_size,i,s);
                int t =rle(v,block_size,i,s);
                s->tamBrle[i] = t;
                fwrite(&v,sizeof(char),t,fw); 
            }
            if (size_of_last_block != block_size){
                n = fread( &v,sizeof(char),size_of_last_block,fp);
                freq(v,size_of_last_block,i,s);
                int t = rle(v,size_of_last_block,i,s);
                s->tamBrle[n_blocks-1] = t;
                fwrite(&v,sizeof(char),t,fw); 
            }
            else {
                n = fread( &v,sizeof(char),size_of_last_block,fp);
                freq(v,tam_b,i,s);
                int t = rle(v,block_size,i,s);
                s->tamBrle[n_blocks-1] = t;
                fwrite(&v,sizeof(char),t,fw); 
            }
        }  
        escreve_freq(s,a);
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
    int count;

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
                    printf ("Tamanho dos blocos analisados no ficheiro RLE: %d/%d bytes\n", s.tamBrle[0],s.tamBrle[s.n_blocos -1]);
                }
                printf ("Tempo de execução do módulo (milissegundos): %ld\n", final);
        
                printf ("Ficheiros gerados: %s %s\n",s.nome, s.nfreq); 
            
            }
        }
    else printf("Não é este módulo\n");
}

