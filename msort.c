#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
     *  Etapa 1: geracao de corridas.
     */

    int *buffer,qtdElementos,tamElemento;

    tamElemento = sizeof(int);
    qtdElementos = memoria/tamElemento;

    // Passo 1: Criar um buffer do tamanho <memoria>.
    buffer = (int *) malloc(qtdElementos*tamElemento);

    int qtdCorridas=0,count =0;
    long int endOfFile=0,pointerFile=0;
    char fileTempName[15];

    // Abre arquivo de entrada para geracao das corridas
    pFile = fopen(fileIn,"r");

    // Pegando fim do arquivo
    fseek(pFile,0,SEEK_END);
    endOfFile = ftell(pFile);
    rewind(pFile);

    while(pointerFile < endOfFile)
    {
        // Carrega os arquivos para a memoria
        printf("--- Carregando dados ---\n");
        count = fread(buffer,tamElemento,qtdElementos,pFile);
        printf("--- numeros lidos %d ---\n",count);

        // Ordenacao com quicksort
        printf("--- Ordenando ---\n");
        quicksort(buffer,0,count-1);

        // Grava uma nova corrida
        sprintf(fileTempName,"%d.tmp",qtdCorridas);
        printf("--- Gerando arquivo %s---\n",fileTempName);
        write(buffer,count,fileTempName);

        pointerFile = ftell(pFile);
        printf("--- Posicao Ponteiro %ld---\n",pointerFile);

        qtdCorridas++;
    }

    // Fecha o arquivo
    fclose(pFile);
    free(buffer);

    /*
     *  Etapa 2: intercalacao das corridas.
     *           kVias + 1 buffers na memoria principal.
     */
     // Passo 1: Encher as kVias com arquivo de corrida.
     // Passo 2: Intercalar kVias no buffer de saida.
     // Passo 3: Se buffer de saida estiver cheio grava num arquivo de saida.
     // Passo 4: Repetir passos 1 a 4 ate os arquivos abertos inicialmente acabarem.
     // Passo 5: Fechar arquivo de saida gerando uma nova corrida com tamanho kvias vezes maior.
     // Passo 6: Repetir passo 1 a 5 ate sobrar apenas um arquivo.

    int **V,*I,tamBuffer,qtdBuffers,i,j,x,y,z,minX,minY;

    FILE *fSorted, **fBuffer;
    char **fileNames;

    qtdBuffers = kVias +1;
    /* Tamanho de cada uma das k vias */
    tamBuffer = memoria/qtdBuffers;

    /* Alocacao dos buffers para intercalacao */
    for(i=0;i<qtdBuffers;i++)
    {
        V[i] = (int *) malloc(tamBuffer*sizeof(int));
    }

    I = (int *) malloc(kVias*sizeof(int));

    fileNames = (char **)malloc(kVias*sizeof(char*));
    for(i=0;i<kVias;i++)
    {
	I[i] = 0; 
	fileNames[i] = (char **) malloc(15*sizeof(char*);
    }

    fBuffer = (FILE **) malloc(kVias*sizeof(FILE *));

    z = 0;
    while(1)
    {
	// carrega buffers
	for(y=0;y<kVias)
	{
	    if(fBuffer[y]==null || feof(fBuffer[y]))
	    {
		//leNovoArquivo de corridas e carrega no buffer de memoria
		sprintd(fileNames[y],"%d.tmp",y);
		fBuffer[y] = fopen(fileNames[y],"r");
		load("trocar-este-nome",V[y],tamBuffer,fBuffer[y]);
	    } else {
		if(I[y]>=tamBuffer)
		{
		    // carrega novos dados a um buffer que jah foi todo consumido pela
		    // intercalacao e zera o indice deste buffer
	    	    load("trocar-este-nome",V[y],tamBuffer,fBuffer[y]);
		    I[y] = 0;
		}
	    }
	}

	// intercala
	for(y=0;y<kVias*tamBuffer;y++)
	{
	    minX = 0;
	    minY = 0;
	    for(x=0;x<kVias;x++)
	    {
		if(I[x]<tamBuffer){
		    if(V[x,I[x]]<V[minX,minY])
		    {
		        minX = x;
		        minY = I[x];
		    }
		}
	    }
	    V[kVias] = V[minX,minY];
	    I[minX]=I[minX] + 1;
	}

	// grava
	write(V[kVias],tamBuffer,"nome-provisorio");

    }

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


/************** Ordenacao *****************/
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
