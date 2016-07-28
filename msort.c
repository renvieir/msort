#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct buffer TBuffer;
struct buffer
{
    int *Dados;
    int tam;
    int i;
    char fileName[15];
    FILE *pFile;
    long int endOfFile;
};

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

void printVetor(int *V, int n)
{
    int i;
    printf("\n");
    for(i=0;i<n;i++)
    {
        printf(" %d ",V[i]);
    }
    printf("\n");
}

/************** Ordenacao *****************/
void troca(int *a, int *b){
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

void sortStruct(TBuffer *bufer)
{
    quicksort(bufer->Dados,0,bufer->tam);
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

int write(int *V,int num, char *fileName)
{
    FILE *pFile;

    pFile = fopen(fileName,"wb");
    int n = fwrite(V,sizeof(int),num,pFile);
    fclose(pFile);
    return n;
}

int fimArquivo(TBuffer *bufer, int y)
{
    if(bufer[y].endOfFile == ftello(bufer[y].pFile)){
        //printf("\nAtingiu EOF da via: %d. i:%d, tam:%d\n",y,bufer[y].i,bufer[y].tam);
        return 1;
    }
    return 0;
}

int fimArquivos(TBuffer *bufer, int vias)
{
    int i,count=0;

    for(i=0;i<vias;i++)
    {
        if(!fimArquivo(bufer,i))
        {
            return 0;
        } else 
        if(bufer[i].tam>0) 
        {
            return 0;
        }
    }
    return 1;
}

void menor(TBuffer *bufer,int viasValidas,int indSaida, int *minVia, int tamBuffer)
{
    int i,count=0;
    
    for(i=0;i<viasValidas;i++)
    {
        // Carrega bufer caso ele esteja sem dados e ainda nao atingiu fim do arquivo
        if(bufer[i].tam==0 && !fimArquivo(bufer,i))
        {
            //printf("\nLendo dados para a via %d\n",i);
            bufer[i].tam = fread(bufer[i].Dados, sizeof(int),tamBuffer,bufer[i].pFile);
        }
        
        // Caso o menor elemento era de uma via que jah acabou e a via 
        // atual nao acabou entao seta a via atual como menor
        if(bufer[*minVia].tam == 0 && bufer[i].tam!=0){
            *minVia = i;
        }
        // só compara o bufer que tem elementos pra comparar
        if(bufer[i].tam>0)
        {
            //printf("\nAinda tem dados na via %d. Dados: %d. %d de %d\n",i,bufer[i].Dados[bufer[i].i],bufer[i].i,bufer[i].tam);
            if(bufer[i].Dados[bufer[i].i] <= bufer[*minVia].Dados[bufer[*minVia].i])
            {
                *minVia = i;
                count++;
            }
        }
    }
    
    // Só insere na saída caso tenha achado algum menor
    if(count>0){
        //printf("\nMenor:%d\n",bufer[*minVia].Dados[bufer[*minVia].i]);
        
        // Insere na saída
        bufer[indSaida].Dados[bufer[indSaida].i] = bufer[*minVia].Dados[bufer[*minVia].i];
        bufer[indSaida].i++;
        bufer[indSaida].tam++;
        
        // Anda o indice do cara que continha o menor
        bufer[*minVia].i++;
        
        // Reseta bufer caso tenha chegado ao fim
        if(bufer[*minVia].i >= bufer[*minVia].tam){
            bufer[*minVia].tam = 0;
            bufer[*minVia].i = 0;
        }
    }    
}

int main( int argc, char** argv )
{    
    verificaParametros(argc);

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
    int qtdElementos,tamElemento,qtdCorridas=0;
    TBuffer *bufer;

    tamElemento = sizeof(int);
    qtdElementos = memoria/tamElemento;

    bufer = (TBuffer*) malloc(sizeof(TBuffer));
    bufer->Dados = (int *) malloc(qtdElementos*tamElemento);
    bufer->tam = qtdElementos;
    bufer->pFile = fopen(fileIn,"rb");

    while(!feof(bufer->pFile))
    {
        // Carrega os arquivos para a memoria
        bufer->tam = fread(bufer->Dados,tamElemento,bufer->tam,bufer->pFile);

        if(bufer->tam > 0)
        {
            // Ordenacao com quicksort
            sortStruct(bufer);
            //printVetor(bufer->Dados,bufer->tam);
            
            // Grava uma nova corrida
            sprintf(bufer->fileName,"%d.tmp",qtdCorridas);            
            write(bufer->Dados,bufer->tam,bufer->fileName);

            qtdCorridas++;
        }
    }
    
    fclose(bufer->pFile);
    printf("------> Fim da geracao de corridas!\n\n");
        
    /*
     * Etapa 2: intercalação de corridas 
     */
    printf("------> Iniciando intercalacao...\n");
    int qtdBuffers,tamBuffer, contCorridas,y,minX, viasValidas=0;
    
    qtdBuffers = kVias + 1;
    tamBuffer = qtdElementos / qtdBuffers;
    
    bufer = (TBuffer *) realloc(bufer,qtdBuffers*sizeof(TBuffer));
    for(y=0;y<qtdBuffers;y++)
    {
        bufer[y].Dados = (int *) malloc(tamBuffer*tamElemento);
        bufer[y].tam = 0;
        bufer[y].i = 0;
    }    
    
    contCorridas = 0;
    while(qtdCorridas - contCorridas > 1)
    {
        //printf("\t------> Iniciando rodada de intercalação...\n");
        // Abrir arquivos de entrada para leitura
        viasValidas = 0;
        for(y=0;y<kVias;y++)
        {
            // Para o caso de nao ter arquivos para todas as kVias
            if(contCorridas<qtdCorridas)
            {
                // Abre arquivos de corrida para leitura
                sprintf(bufer[y].fileName,"%d.tmp",contCorridas);
                bufer[y].pFile = fopen(bufer[y].fileName,"rb");
                
                // Pega o ponteiro para o fim do arquivo 
                fseeko(bufer[y].pFile,0,SEEK_END);
                bufer[y].endOfFile = ftello(bufer[y].pFile);
                rewind(bufer[y].pFile);
                
                // Seta o tamanho do bufer
                bufer[y].tam = fread(bufer[y].Dados,sizeof(int),tamBuffer,bufer[y].pFile);
                
                // conta corrida lida
                if(bufer[y].tam>0)
                {
                    contCorridas++;
                }
                viasValidas++;
            }
        }
        
        // Abrir arquivo de saida para escrita
        sprintf(bufer[kVias].fileName,"%d.tmp",qtdCorridas);
        bufer[kVias].pFile = fopen(bufer[kVias].fileName,"wb");        
        
        minX = 0;

        // Intercala até o fim dos arquivos de entrada
        while(!fimArquivos(bufer,viasValidas))
        {
            menor(bufer,viasValidas,kVias,&minX,tamBuffer);                

            if((bufer[kVias].i==tamBuffer) || fimArquivos(bufer,viasValidas))
            {
                // Gravar buffer de saida
                //printVetor(bufer[kVias].Dados,bufer[kVias].tam);
                fwrite(bufer[kVias].Dados,sizeof(int),bufer[kVias].tam,bufer[kVias].pFile);
                bufer[kVias].i=0;
                bufer[kVias].tam=0;
                //printf("\nGravou no arquivo de saida %d.tmp\n",qtdCorridas);
            }
        }
        
        // fechar buffers de entrada
        for(y=0;y<viasValidas;y++)
        {
            //printf("\nbuffer:%d,i:%d,tam:%d\n",y,bufer[y].i,bufer[y].tam);
            fclose(bufer[y].pFile);
            remove(bufer[y].fileName);
        }
        
        // fecha buffer de saida
        fclose(bufer[kVias].pFile);
        
        // Após gravar saída uma nova corrida foi gerada
        qtdCorridas++;
        //printf("\t------> Terminando rodada de intercalação. Restam: %d corridas a intercalar\n",qtdCorridas-contCorridas);
    }
    
    // TODO mudar o nome da corrida (qtdCorridas -1) para o nome do arquivo de saida fornecido pelo usuario
    // printf("------> %s %s ",bufer[kVias].fileName,fileOut);
    rename(bufer[kVias].fileName,fileOut);
    
    printf("------> Fim da intercalacao de corridas!\n\n");
    
    return 0;
}
