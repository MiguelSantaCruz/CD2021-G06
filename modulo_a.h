/* Autores: João Gonçalves - A93204 , Maria Gomes - A93314
Data: 03/01/2020

Ficheiro contém a função fsize alterada para que estabelece-se um tamanho minimo para o ficheiro 
recebido e para o tamanho dos blocos de um 1024 bytes.
Tem a nossa estrutura de dados e os prototipos das funções criadas.
    
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FSIZE_DEFAULT_BLOCK_SIZE 65536        // Default block size = 64 KBytes
#define FSIZE_MIN_BLOCK_SIZE 1024              // Min block size = 1 KBytes
#define FSIZE_MAX_BLOCK_SIZE 67108864           // Max block size = 64 MBytes
#define FSIZE_MAX_NUMBER_OF_BLOCKS 4294967296   // Max number of blocks that can be returned = 2^32 blocks
#define FSIZE_MAX_SIZE_FSEEK 2147483648         // Max size permitted in fseek/ftell functions = 2 GBytes
#define FSIZE_ERROR_BLOCK_SIZE -1               // Error: Block size is larger than max value
#define FSIZE_ERROR_NUMBER_OF_BLOCKS -2         // Error: Number of Blocks exceeds max value permitted
#define FSIZE_ERROR_IN_FILE -3                  // Error: Opening or reading file
#define FSIZE_ERROR_IN_FTELL -1L                // Error: When using ftell()

long long fsize(FILE *fp_in, unsigned char *filename, unsigned long *the_block_size, long *size_of_last_block)
{   
    unsigned long long total;
    long long n_blocks; 
    unsigned long block_size;
    int fseek_error;
    FILE *fp;

    block_size = *the_block_size;
    if (block_size > FSIZE_MAX_BLOCK_SIZE) return (FSIZE_ERROR_BLOCK_SIZE);
    if (block_size == 0UL) block_size = FSIZE_DEFAULT_BLOCK_SIZE;
    if (block_size < FSIZE_MIN_BLOCK_SIZE) block_size = FSIZE_MIN_BLOCK_SIZE;
    *the_block_size = block_size;

    if (filename == NULL || *filename == 0) fp = fp_in;
    else
    { fp = fopen(filename, "rb");
      if (fp == NULL) return (FSIZE_ERROR_IN_FILE);
    }

    fseek_error = fseek(fp, 0L, SEEK_SET);
    if (fseek_error) return (FSIZE_ERROR_IN_FILE);

    fseek_error = fseek(fp, 0L, SEEK_END);
    if (!fseek_error)
    { total = ftell(fp);
      if (total == FSIZE_ERROR_IN_FTELL || total < FSIZE_MIN_BLOCK_SIZE) return (FSIZE_ERROR_IN_FILE);
      n_blocks = total/block_size;
    
      if (n_blocks*block_size == total) *size_of_last_block = block_size;
      else
      { 
        if (total - n_blocks*block_size < FSIZE_MIN_BLOCK_SIZE){
          *size_of_last_block = (total - n_blocks*block_size) + block_size;
          
        }else{
          *size_of_last_block = (total - n_blocks*block_size);
          n_blocks++;
        }
      }
      
      return(n_blocks);
    }
}

 struct freq_bloco {
    int frequencia [256];
};

typedef struct {
    int rle; // se a frequencia vem dum ficheiro rle, rle = 1    
    int taxaC;
    int n_blocos;
    int tamBrle [100];//tamanhos dos blocos do rle
    int tamB;//tamanho do primeiro bloco sem rle
    int tamU;//tamanho do ultimo bloco sem rle
    char nome [50];
    char nfreq [54];
    struct freq_bloco * tab;
}Stack;


void escreve_freq (Stack *s, char a []);
void freq (unsigned char v [], unsigned long tamBloco, int b, Stack * s);
unsigned long rle (unsigned char v [], unsigned char * f, unsigned long tambloco);
int taxaCompressao (unsigned long tam_I,unsigned long tam_F);
int ler_ficheiro (char fic [],unsigned long tam_b, char a [],Stack *s,int r);
