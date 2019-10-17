#include<stdio.h>
#include<stdlib.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
};
struct fila
{
    int vertice;
    struct fila *prox;    
};

typedef struct fila Fila;
typedef struct grafos Grafos;

int menu();
Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);
void buscaEmLargura(Grafos *gr, int raiz);
Fila *ColocaVerticeNaFila(Fila *F, int raiz);
Fila *RemoveVerticeNaFila(Fila *F);
int VerificaSeTaNaFila(Fila *F, int elemento);
void exibeFila(Fila *F);

void main(){
    Grafos *gr;
    int choice, vertice, ponderado, digrafo, peso, origem, destino;
    /*do{
        choice = menu();
        switch(choice){
            case 0:
                break;
            case 1:
                printf("Digite o numero de vertices:\n");
                scanf("%d", &vertice);
                printf("Eh ponderado?[1-Sim/0-Nao]:\n");
                scanf("%d", &ponderado);
                gr = criaGrafo(vertice, ponderado);
                break;
            case 2:
                printf("Eh digrafo?[1-Sim/2-Nao]:\n");
                scanf("%d", &digrafo);
                break;
        }
    }while(choice != 0);*/
    gr = criaGrafo(6, 0);
    insereAresta(&(gr), 6, 4, 0, 0);
    insereAresta(&(gr), 3, 2, 0, 0);
    insereAresta(&(gr), 4, 5, 0, 0);
    insereAresta(&(gr), 4, 3, 0, 0);
    insereAresta(&(gr), 5, 1, 0, 0);
    insereAresta(&(gr), 5, 2, 0, 0);
    insereAresta(&(gr), 1, 2, 0, 0);
    buscaEmLargura(gr, 6);
}

int menu(){
    int choice;
    printf("1-Cria Grafo\n2-Insere Aresta\n0-Sair\n");
    scanf("%d", &choice);
    return choice;
}

Grafos* criaGrafo(int vertice, int ehPonderado){
    Grafos *gr;
    int i;
    gr = (Grafos*)malloc(sizeof(Grafos));
    if(gr != NULL){
        gr->nVertices = vertice;
        gr->ehPonderado = (ehPonderado != 0)? 1:0;
        gr->grau = (int*) calloc(sizeof(int), vertice);
        gr->arestas = (int**) malloc(vertice * sizeof(int*));
        for(i=0; i<vertice; i++){
            gr->arestas[i] = (int*) calloc(vertice, sizeof(int));
        }
        if(gr->ehPonderado){
            gr->pesos = (int**) malloc(vertice * sizeof(int*));
            for(i=0; i<vertice; i++){
                gr->pesos[i] = (int*) calloc(vertice, sizeof(int));
            }   
        }
    }
    return gr;
}

void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo){
    if(*gr != NULL){
        if(origem > 0 && origem <= (*gr)->nVertices){
            if(destino > 0 && destino <= (*gr)->nVertices){
                (*gr)->arestas[origem-1][(*gr)->grau[origem-1]] = destino;
            }
            if((*gr)->ehPonderado){
                (*gr)->pesos[origem-1][(*gr)->grau[origem-1]] = peso;
            }
            (*gr)->grau[origem-1]++;
            if(!ehDigrafo){
                insereAresta(gr, destino, origem, peso, 1);
            }
        }
    }
}

void buscaEmLargura(Grafos *gr, int raiz){
    Fila *F=NULL;
    int qtVerticesMarcados = 0;
    int vertice1;
    int *pointer;
    int *marcados;
    int elemento;

    marcados = (int*)calloc(gr->nVertices, sizeof(int));

    marcados[raiz-1] = 1;
    qtVerticesMarcados++;

    F = ColocaVerticeNaFila(F, raiz);
    
    while(F!=NULL){
        vertice1 = F->vertice;
        pointer = gr->arestas[vertice1-1];
        for(int i=0; i<gr->grau[vertice1-1]; i++){
            elemento = pointer[i];
            if(marcados[elemento-1]==0){
                printf("%d visitou %d.\n", vertice1, elemento);
                marcados[elemento-1]=1;
                F=ColocaVerticeNaFila(F, elemento);
            //    exibeFila(F);
            }else if(VerificaSeTaNaFila(F, elemento)){
                printf("%d visitou %d.\n", vertice1, elemento);
            }
        }
        F=RemoveVerticeNaFila(F);        
        //exibeFila(F);                
    }
}

Fila *ColocaVerticeNaFila(Fila *F, int raiz){
    Fila *novo, *aux;
    novo = (Fila*)malloc(sizeof(Fila));

    novo->vertice = raiz;

    if(F==NULL){
        F = novo;
        novo->prox = NULL;
    }else{
        for(aux=F; aux->prox!=NULL; aux=aux->prox);
        aux->prox=novo;
        novo->prox = NULL;
    }
    
    return F;
}

Fila *RemoveVerticeNaFila(Fila *F){
    Fila *aux;

    aux = F->prox;
    F = NULL;
    free(F);

    return aux;
}

int VerificaSeTaNaFila(Fila *F, int raiz){
    Fila *aux = F;
    int retorno = 0;

    for(aux; aux!=NULL; aux=aux->prox){
        if(aux->vertice==raiz){
            retorno=1;
        }
    }
    return retorno;
}

void exibeFila(Fila *F){
    Fila *aux = F;
    for(aux;aux!=NULL;aux=aux->prox)
        printf("%d ", aux->vertice);
    puts("\n");
}