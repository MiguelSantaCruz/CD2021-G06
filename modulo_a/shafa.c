/* Autores: João Gonçalves - A93204 , Maria Gomes - A93314
Data: 03/01/2020
Este ficheiro contém a main do módulo A. 
*/



#include "modulo_a.c"

void main (int argc, char *argv[]) {
    clock_t begin = clock();
    float final;

    Stack s;
    int tam = 65536; // tamanho por defeito 64kB
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
                    if (strcmp (argv[d], "-b") == 0){
                        if (strcmp (argv[d+1], "K") == 0) tam = 655360; //640 kB 
                        if (strcmp (argv[d+1], "m") == 0) tam = 8388608; //8MB 
                        if (strcmp (argv[d+1], "M") == 0) tam = 67108864; //64MB
                    }
                }
            }
            
            int c = ler_ficheiro (argv[1],tam, a,&s,r);           
            
            clock_t end = clock();
            final = ((float)(end - begin))*1000/ CLOCKS_PER_SEC;

            printf ("José Gonçalves, Maria Gomes, MIEI/CD, 1-jan-2021\nModulo: f (cálculo das frequências dos símbolos)\n");
            if (c == 1){
                printf ("Número de Blocos : %d\n", s.n_blocos);                
                printf ("Tamanho dos blocos analisados no ficheiro Original: %d/%d bytes\n",s.tamB,s.tamU);
                if (r == 1 || s.rle == 1) {
                    printf ("Compressão RLE: %s (%d por cento de compressão)\n",s.nome, s.taxaC);
                    printf ("Tamanho dos blocos analisados no ficheiro RLE: %d/%d bytes\n", s.tamBrle[0],
                                    ((s.n_blocos == 1 ) ? 0 : (s.tamBrle[s.n_blocos -1])));
                }
                printf ("Tempo de execução do módulo (milissegundos): %.2f\n", final);
        
                printf ("Ficheiros gerados: %s %s\n",s.nome, s.nfreq); 
            
            }
        }
    else printf("Não é este módulo\n");
    
    //return 0;
}
