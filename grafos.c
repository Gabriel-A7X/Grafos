#include<stdio.h>
#include<stdlib.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
};

typedef struct grafos Grafos;

int menu();
Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);

void main(){
    Grafos *gr;
    int choice, vertice, ponderado, digrafo, peso, origem, destino;
    do{
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
    }while(choice != 0);
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
    gr = (Grafos*) malloc(sizeof(Grafos));
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
