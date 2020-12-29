#include "modulo_a.h"



void escreve_freq (Stack *s, char a []){
    FILE *f;
    if (s->rle){
        char or [] = ".freq";
        strcat(a,or);
        f = fopen (a,"w");
        fprintf (f,"@R");
    }
    else {
        char or [] = ".freq";
        strcat(a,or);
        f = fopen (a,"w");
        fprintf (f,"@N");
    }   
    fprintf (f,"@%d",s->n_blocos);

    for (int r =0; r < s->n_blocos -1; r++){
        fprintf (f,"@%d@",s->tam_b[r]);
        fprintf (f,"%d;", (s->tab)[r].frequencia[0]);
        for(int i=1;i<255;i++) {
            int ant = (s->tab)[r].frequencia[i-1];
            int prox = (s->tab)[r].frequencia[i];
            if (prox == ant )fprintf (f,";");
            else fprintf (f,"%d;", (s->tab)[r].frequencia[i]);
    
        }
    }

    fprintf (f,"@%d@",s->tam_U);
    fprintf (f,"%d;", (s->tab)[s->n_blocos-1].frequencia[0]);
    for(int i=1;i<255;i++) {
        int ant = (s->tab)[s->n_blocos-1].frequencia[i-1];
        int prox = (s->tab)[s->n_blocos-1].frequencia[i];
        if (prox == ant )fprintf (f,";");
        else fprintf (f,"%d;",(s->tab)[s->n_blocos-1].frequencia[i]);
    }
    fprintf (f,"@0");
}



void freq (unsigned char v [], int n, int pB, Stack * s){
  
    int i, freq = 0, p=0;
    int fq[256];
    
    for (i = 0; i < 255; i++){
        
        for(int p = 0; p < n; p++){
            if (v[p] == i) freq++;
            
        }
        s->tab[pB].frequencia[i] = freq;// fq[i] = freq;
        freq = 0;
    } 
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
    return (r<0?4:r);
}


void ler_ficheiro (char fic [],unsigned long tam_b, char a []){
    FILE *f = fopen(fic,"rb");
    unsigned long long total;
    long long n_blocks;
    unsigned long size_of_last_block = tam_b,block_size = tam_b;

    n_blocks = 2;//fsize(f, NULL, &block_size, &size_of_last_block);
    //controlo do tamanho do último bloco

    int n = 0;
    FILE *fp = fopen(fic,"rb");
    
    unsigned char v [tam_b*2]; 
    
    Stack s;
    s.tab = calloc(n_blocks, sizeof(struct freq_b));
    s.rle = 0;
    s.n_blocos = n_blocks;
    
    
    n = fread( &v,sizeof(char),tam_b,fp);// le só o primeiro bloco
    freq(v,tam_b,0,&s);

    int rl = rle(v,tam_b,0,&s);
    
    int taxa = taxaCompressao(tam_b, rl);
    printf("%d\n",taxa);
    int i;
    if (taxa < 5){ // não faz o rle
        s.tam_b[0] = tam_b;
        s.tam_U = size_of_last_block;
        for (i = 1; i<= n_blocks - 2; i++){
            n = fread(&v,sizeof(char),tam_b,fp);
            freq(v,tam_b,i,&s);
            s.tam_b[i] = tam_b;
        }
        
        if (size_of_last_block != tam_b){
            n = fread( &v,sizeof(char),size_of_last_block,fp);
            freq(v,size_of_last_block,i,&s);
        }
        else {
            fread(&v,sizeof(char),tam_b,fp);
            freq(v,tam_b,i,&s);
        
        }
        
    }
    else {
        s.tam_b[0] = rl;
        
        char or [] = ".rle";
        strcat(a,or);
        FILE *fw = fopen (a,"wb");
        s.rle = 1;
        fwrite(&v,sizeof(char),rl,fw);//escreve o .rle

        for (i = 1; i<= n_blocks - 2; i++){
            n = fread( &v,sizeof(char),tam_b,fp);
            freq(v,tam_b,i,&s);
            int t =rle(v,tam_b,i,&s);
            s.tam_b[i] = t;
            fwrite(&v,sizeof(char),t,fw); 
        }
        if (size_of_last_block != tam_b){
            n = fread( &v,sizeof(char),size_of_last_block,fp);
            freq(v,size_of_last_block,i,&s);
            int t = rle(v,size_of_last_block,i,&s);
            s.tam_U = t;
            fwrite(&v,sizeof(char),t,fw); 
        }
        else {
            n = fread( &v,sizeof(char),size_of_last_block,fp);
            freq(v,tam_b,i,&s);
            int t = rle(v,tam_b,i,&s);
            s.tam_U = t;
            fwrite(&v,sizeof(char),t,fw); 
        }
    }  
    escreve_freq(&s,a);
}




int main (int argc, char *argv []){
    int p = strlen(argv[1]);
    char a [p + 8];
    strcpy(a,argv[1]);
    int tam = atoi (argv[2]);//em bytes
    if (argc == 3) ler_ficheiro (argv[1],tam,a);
    else ler_ficheiro (argv[1],65536,a); 

}










//codigo interpretador



void main (int argc, char *argv[]) {
    clock_t begin = clock();
    int count;
    int tamanho;
    unsigned long size_of_last_block, block_size = tam_b;
    n_blocks = fsize(f, NULL, &block_size, &size_of_last_block);        //alterar para o caso de receber um -b
    printf("argc = %d\n", argc);

    if (argc != 7) printf ("ERROR!\nO comando não é valido!\n");
    else if (strcmp (argv[1], "Shafa") == 0 
            && strcmp (argv[3], "-m") == 0
            && strcmp (argv[4], "f") == 0
            && strcmp (argv[5], "-c") == 0
            && strcmp (argv[6], "r") == 0) {
//                ler_ficheiro (argv[2], 20);                 //funçao size que calcula otamanho do ficheiro
           
            printf ("José Gonçalves, Maria Gomes, MIEI/CD, 1-jan-2021\n");


            printf ("Modulo: f (cálculo das frequências dos símbolos)\n");


            printf ("Número de Blocos : %d", n_blocks );                 //falta fazer
            

            //printf ("Tamanho dos blocos analisados no ficheiro Original: 57444/1620 bytes");

            int tamF = rle ()
            printf ("Compressão RLE: exemplo.txt.rle (%d  compressão", taxaCompressao (tamI, tamF));


            //printf ("Tamanho dos blocos analisados no ficheiro RLE: 57444/1620 bytes");     //falta fazer (usar a fsize)
            
            
            clock_t end = clock();
            double final = (double)(end - begin) / CLOCKS_PER_SEC;
            final = (double) final * 1000;
            printf ("Tempo de execução do módulo (milissegundos): %f\n", final);          //falta fazer
            
            
            //printf ("Ficheiros gerados: %s, %s", ficheiro1, ficheiro2);                     //falta fazer
            

            printf ("OK\n");
        }
}


