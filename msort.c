#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void verificaParametros(int argc);
int load(int *V,int num,FILE *pFile);
int write(int *V,int num, char *fileName);
void quicksort(int* V,int ini, int fim);
int fimArquivos(FILE **fBuffer, int numBuffers);
void print(int *V, int n)
{
    int i;
	for(i=0;i<n;i++)
	{
	    printf(" %d ",V[i]);
	}
}

int indicesZerados(int *I, int kVias)
{
    int i=0;
    for(i=0;i<kVias;i++)
    {
	if(I[i]!=0)
	    return 0;
    }
    return 1;
}

int indicesTerminados(int *I, int kVias, int tamBuffer)
{
    int i=0;
    for(i=0;i<kVias;i++)
    {
	if(I[i] < tamBuffer)
	    return 0;
    }
    return 1;
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
    printf("------> Iniciando geracao de corridas ...\n");
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
        printf("\t--- Carregando dados ---\n");
        count = fread(buffer,tamElemento,qtdElementos,pFile);
        printf("\t--- numeros lidos %d ---\n",count);

        // Ordenacao com quicksort
        printf("\t--- Ordenando ---\n");
        quicksort(buffer,0,count-1);

        // Grava uma nova corrida
        sprintf(fileTempName,"%d.tmp",qtdCorridas);
        printf("\t--- Gerando arquivo %s---\n",fileTempName);
        write(buffer,count,fileTempName);

        pointerFile = ftell(pFile);
        printf("\t--- Posicao Ponteiro %ld --- fim %ld\n",pointerFile,endOfFile);

        qtdCorridas++;
    }

    // Fecha o arquivo
    fclose(pFile);
    free(buffer);

    printf("------> Fim da geracao de corridas!\n\n");
    printf("------> Iniciando intercalacao...\n");
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

    int **V,*I,tamBuffer,qtdBuffers,i,j,x,y,corridaCont,minX,minY;

    FILE *fSorted, **fBuffer;
    char **fileNames;

    qtdBuffers = kVias +1;
    /* Tamanho de cada uma das k vias */
    tamBuffer = memoria/qtdBuffers;

    V = (int **) malloc(qtdBuffers*sizeof(int *));
    fileNames = (char **)malloc(kVias*sizeof(char*));
    I = (int *) malloc(kVias*sizeof(int));
    fBuffer = (FILE **) malloc(kVias*sizeof(FILE *));

    /* Alocacao dos buffers para intercalacao */
    for(i=0;i<qtdBuffers;i++)
    {
        V[i] = (int *) malloc(tamBuffer*sizeof(int));

        // estruturas auxiliares
        if(i<qtdBuffers-1)
        {
            I[i] = 0;
            fileNames[i] = (char *) malloc(15*sizeof(char));
        }
    }

    // contador de corridas lidas
    corridaCont = 0;

    // esquanto houver corridas nao lidas
    while(corridaCont < qtdCorridas)
    {
        printf("\n corridaCont %d qtdCorridas %d\n",corridaCont,qtdCorridas);

        // carrega novos buffers
        for(y=0;y<kVias;y++)
        {
            // So carrega novo arquivo quando todos os arquivos tiverem sido lidos por completo
            // ou no inicio do loop
            if(indicesZerados(I,kVias) || indicesTerminados(I,kVias, tamBuffer))
            {
                //printf("\nErro aqui 1\n");
                //leNovoArquivo de corridas e carrega no buffer de memoria

                //if(corridaCont < qtdCorridas){

                // fecha o arquivo atual antes de abrir outro arquivo
                fclose(fBuffer[y]);

                sprintf(fileNames[y],"%d.tmp",corridaCont);
                fBuffer[y] = fopen(fileNames[y],"r");
                load(V[y],tamBuffer,fBuffer[y]);
                I[y]=0;
                //}
                /*else {
                    fBuffer[y] = NULL;
                    I[y] = tamBuffer;
                }*/

                // incrementa contador de corridas lidas
                corridaCont++;
            }
        }

        // intercala todos os elementos dos kVias arquivos
        //for(y=0;y<kVias*tamBuffer;y++)
        y=0;
        while(!fimArquivos(fBuffer,kVias))
        {
            // indice do menor numero
            minX = 0;
            minY = 0;

            // comparação das kVias
            for(x=0;x<kVias;x++)
            {
            // se jah consumiu todo o buffer e o arquivo ainda nao acabou
            // entao carrega mais dados pro buffer

            //printf("x=%d => indice:%d, tamBufer:%d\n",x,I[x],tamBuffer);

                // leitura de buffer que jah foi consumido
                if(I[x]>=tamBuffer && !feof(fBuffer[x]))
                {
                    load(V[x],tamBuffer,fBuffer[x]);
                    I[x]=0;
                }

                // acha o menor somente entre aqueles que nao atingiram o fim do arquivo
                if(I[x] < tamBuffer)
                {
                    if(V[x,I[x]]<V[minX,minY])
                    {
                        minX = x;
                        minY = I[x];
                    }
                }
            }

            // preenche a saida com o menor
            V[kVias,y%tamBuffer] = V[minX,minY];

            // anda o indice do buffer que continha o menor valor
            I[minX] = I[minX] + 1;

            // grava o arquivo quando enche o buffer
            if(y>0 && y%(tamBuffer)==0)
            {
                sprintf(fileTempName,"%d.tmp",qtdCorridas);
                write(V[kVias],tamBuffer,fileTempName);
            }
            y++;

            printf("\t--- arquivosLidos: %d arquivosTotal: %d\n",corridaCont,qtdCorridas);
        }

        qtdCorridas++;

    }

}

int fimArquivos(FILE **fBuffer, int numBuffers)
{
    int i;
    for (i=0;i<numBuffers;i++)
    {
        if(!feof(fBuffer[i]))
            return 0;
    }
    return 1;
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

int load(int *V,int num, FILE *pFile)
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
