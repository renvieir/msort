#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Verifica se os parâmetros foram passados de forma correta ao programa */
void verificaParametros(int argc);
int load(char *fileName, int *V,int num,FILE *pFile);
int write(int *V,int num, char *fileName);
void quicksort(int* V,int ini, int fim);

void print(int *V, int n)
{
    int i;
	for(i=0;i<n;i++)
	{
	    printf(" %d ",V[i]);
	}
}

int main( int argc, char** argv )
{    
    verificaParametros(argc);

    FILE *pFile;
    int memoria,kVias;
    char *fileIn,*fileOut;
    
    fileIn = argv[1];
    fileOut = argv[2];
    memoria = atoi(argv[3]);
    kVias = atoi(argv[4]);
    
    /*  
     *  Etapa 1: geração de corridas.
     *           Apenas um buffer na memoria principal.
     *  
     *  Passo 1: Criar um buffer do tamanho <memoria>.
     *  Passo 2: Encher buffer com dados do arquivo de entrada.
     *  Passo 3: Ordenar buffer com um algoritmo de ordenação estável.
     *  Passo 4: Gravar buffer em um arquivo.
     *  Passo 5: Repetir Passos 2 e 4 até o fim do arquivo de entrada.
     */

    int *buffer,qtdElementos,tamElemento;

    tamElemento = sizeof(int);
    qtdElementos = memoria/tamElemento;
    
    // Passo 1: Criar um buffer do tamanho <memoria>.
    buffer = (int *) malloc(qtdElementos*tamElemento);
    
    int i=0,count =0;
    long int endOfFile=0,pointerFile=0;
    char fileTempName[25];
    
    // Abre arquivo de entrada para geração das corridas
    pFile = fopen(fileIn,"r");
    fseek(pFile,0,SEEK_END);
    endOfFile = ftell(pFile);
    rewind(pFile);

    while(pointerFile < endOfFile)
    {
        // Carrega os arquivos para a memoria
        printf("--- Carregando dados ---\n");    
        count = fread(buffer,tamElemento,qtdElementos,pFile);
        printf("--- numeros lidos %d ---\n",count);    
        
        // Ordenação com quicksort
        printf("--- Ordenando ---\n");    
        quicksort(buffer,0,count-1);
        
        // Grava uma nova corrida
        sprintf(fileTempName,"%d.tmp",i);       
        printf("--- Gerando arquivo %s---\n",fileTempName);    
        write(buffer,count,fileTempName);        
        
        pointerFile = ftell(pFile);
        printf("--- Posicao Ponteiro %ld---\n",pointerFile);    
        
        i++;
    }
    
    // Fecha o arquivo
    fclose(pFile);

    /*
     *  Etapa 2: intercalação das corridas.
     *           kVias + 1 buffers na memória principal.
     */
     // Passo 1: Encher as kVias com arquivo de corrida.
     // Passo 2: Intercalar kVias no buffer de saida.
     // Passo 3: Se buffer de saida estiver cheio grava num arquivo de saida.
     // Passo 4: Repetir passos 1 a 4 até os arquivos abertos inicialmente acabarem.
     // Passo 5: Fechar arquivo de saida gerando uma nova corrida com tamanho kvias vezes maior.
     // Passo 6: Repetir passo 1 a 5 até sobrar apenas um arquivo.
    
    /* Tamanho de cada uma das k vias */
    //tamBuffer = memoria/qtdBuffers;
    
    /* Alocação dos buffers para intercalação */
    /*int i;
    for(i=0;i<qtdBuffers;i++)
    {
        V[i] = (int *) malloc(tamBuffer*sizeof(int));
    }*/
}

void verificaParametros(int argc)
{
    if ( argc != 5 )
    {
        fprintf( stderr, "\n\n\t------> Erro!!! <-------\n\n" );
        fprintf( stderr, "\tExecução correta: ./msort <entrada> <saida> <memoria> <kvias>\n\n" );
        fprintf( stderr, "\t\t<entrada>: Nome do arquivo com dados desordenados\n" );
        fprintf( stderr, "\t\t<saida>: Nome do arquivo com dados ordenados\n" );
        fprintf( stderr, "\t\t<memoria>: Quantidade de memoria disponivel em bytes\n" );
        fprintf( stderr, "\t\t<kvias>: Número de vias usadas pelo Merge-Sort\n" );
        exit( 0 );
    }
}

int load(char *fileName, int *V,int num, FILE *pFile)
{
    return fread(V,sizeof(int),num,pFile);    
}

int write(int *V,int num, char *fileName)
{
    FILE *pFile;
    
    pFile = fopen(fileName,"w");    
    int n = fwrite(V,sizeof(int),num,pFile);    
    fclose(pFile);
    return n;
}


/************** Ordenação *****************/
troca(int *a, int *b){
	int aux;
	aux=*a;
	*a=*b;
	*b=aux;
}

void quicksort(int* V,int ini, int fim){
	int pivo;

	if(ini<fim){
		pivo=particao(V,ini,fim);
		quicksort(V,ini,pivo-1);
		quicksort(V,pivo+1,fim);
	}

}

int particao(int* V,int ini, int fim)
{
	int i,pivo,ord;
	pivo=V[fim]; // Pivo eh o ultimo elemento do vetor

	ord=ini-1;
	for(i=ini;i<fim;i++){
		if(V[i]<=pivo){
			++ord;
			troca(&V[ord],&V[i]);
		}
	}
	troca(&V[ord+1],&V[fim]);
	return (ord+1);
}
