#include<stdio.h>
#include<stdlib.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
};
struct fila{
    int vertice;
    struct fila *prox;    
};
struct pilha{
    int n;
    struct pilha *prox;
};

struct maior{
    int tam;
    struct pilha *caminho;
};

typedef struct fila Fila;
typedef struct grafos Grafos;
typedef struct pilha Pilha;
typedef struct maior Maior;

int menu();
Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);
void buscaEmLargura(Grafos *gr, int raiz);
Fila *ColocaVerticeNaFila(Fila *F, int raiz);
Fila *RemoveVerticeNaFila(Fila *F);
int VerificaSeTaNaFila(Fila *F, int elemento);
void exibeFila(Fila *F);
void buscaProf(Grafos *gr, int raiz, int *visitado, Maior *cam, int money);
void buscaEmProfundidade(Grafos *gr, int raiz, int *visitado, Pilha *caminho, int cont, Maior *cam, int money);
void listarAdj(Grafos *gr);
void push(int v, Pilha **p);
void pop(Pilha **p);
void copiaPilha(Pilha **p1,Pilha **p2);
void mostraCam(Pilha *caminho);
void LiberaP(Pilha **p);
int fimDeRota(Grafos *gr, int raiz, int money, int *visitado);

int rota=0;

void main(){
    Grafos *gr;
    int choice, vertice, ponderado, digrafo, peso, origem, destino, qtdVisitados = 0;
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
    gr = criaGrafo(6, 1);
    insereAresta(&(gr), 5, 3, 2, 0);
    insereAresta(&(gr), 2, 1, 5, 0);
    insereAresta(&(gr), 3, 4, 4, 0);
    insereAresta(&(gr), 3, 2, 6, 0);
    insereAresta(&(gr), 4, 0, 2, 0);
    insereAresta(&(gr), 4, 1, 1, 0);
    insereAresta(&(gr), 0, 1, 3, 0);
    int vis[6];
    Maior *cam=(Maior*)malloc(sizeof(Maior));
    cam->tam=0;
    cam->caminho=NULL;
    //listarAdj(gr);
    // buscaProf(gr, 0, vis, cam, 12);
    
    buscaEmLargura(gr, 5);
    // mostraCam(cam->caminho);
    // printf("\n");
}

int menu(){
    int choice;
    printf("1-Cria Grafo\n2-Insere Aresta\n0-Sair\n");
    scanf("%d", &choice);
    return choice;
}

void mostraCam(Pilha *caminho){
    if(caminho==NULL){
        printf("O melhor caminho é:\n");
    }else{
        mostraCam(caminho->prox);
        printf("%d ",caminho->n);
    }
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
        if(origem >= 0 && origem <= (*gr)->nVertices){
            if(destino > 0 && destino <= (*gr)->nVertices){
                (*gr)->arestas[origem][(*gr)->grau[origem]] = destino;
            }
            if((*gr)->ehPonderado){
                (*gr)->pesos[origem][(*gr)->grau[origem]] = peso;
            }
            (*gr)->grau[origem]++;
            if(!ehDigrafo){
                insereAresta(gr, destino, origem, peso, 1);
            }
        }
    }
}

/*void buscaEmProfundidade(Grafos *gr, int raiz, int *visitado, int cont){
    int i;
    visitado[raiz] = -1;
    for(i=0; i<=gr->grau[raiz]; i++){
        if(!visitado[gr->arestas[raiz][i]]){
            printf("%d Visitou %d\n", raiz+1, gr->arestas[raiz][i]+1);
            buscaEmProfundidade(gr, gr->arestas[raiz][i], visitado, cont+1);
        }
    }
}*/

void buscaEmProfundidade(Grafos *gr, int raiz, int *visitado, Pilha *caminho, int cont, Maior *cam, int money){
    int i;
    visitado[raiz] = -1;
    push(raiz,&caminho);
    for(i=0; i<gr->grau[raiz]; i++){
        if(!visitado[gr->arestas[raiz][i]] && (money - gr->pesos[raiz][i])>=0){
            //printf("%d Visitou %d\n", raiz, gr->arestas[raiz][i]);
            buscaEmProfundidade(gr, gr->arestas[raiz][i], visitado, caminho, cont+1, cam, (money - gr->pesos[raiz][i]));
        }
    }
    /*if(fimDeRota(gr,raiz,money,visitado)){
        mostraCam(caminho);
        printf("\n");
        rota++;*/
        if(cont>cam->tam){
            LiberaP(&cam->caminho);
            copiaPilha(&caminho,&cam->caminho);
            cam->tam=cont;
        }
    //}
    pop(&caminho);
    visitado[raiz]=0;
}

/*int fimDeRota(Grafos *gr, int raiz, int money, int *visitado){
    int ret=0;
    for(int i=0; i<gr->grau[raiz]; i++){
        if((money - gr->pesos[raiz][i])<0){
            ret+=1;
        }
    }
    if(ret==gr->grau[raiz]){
        ret=1;
    }
    return ret;
}*/

void buscaProf(Grafos *gr, int raiz, int *visitado, Maior *cam, int money){
    int i, cont = 1;
    for(i=0; i<gr->nVertices; i++){
        visitado[i] = 0;
    }
    Pilha *Caminho=(Pilha*)malloc(sizeof(Pilha));
    Caminho=NULL;
    cont=0;
    buscaEmProfundidade(gr, raiz, visitado, Caminho, cont, cam, money);
}

void listarAdj(Grafos *gr){
    for(int i=0;i<gr->nVertices;i++){
        printf("adjacencias de %d\n",i);
        for(int j=0;j<gr->grau[i];j++){
            printf("%d ",gr->arestas[i][j]);
        }
        printf("\n");
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
    marcados[raiz] = 1;
    qtVerticesMarcados++;
    F = ColocaVerticeNaFila(F, raiz);
    while(F!=NULL){
        vertice1 = F->vertice;
        pointer = gr->arestas[vertice1];
        for(int i=0; i<gr->grau[vertice1]; i++){
            elemento = pointer[i];
            if(marcados[elemento]==0){
                printf("%d visitou %d.\n", vertice1, elemento);
                marcados[elemento]=1;
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

void push(int v, Pilha **p){
    Pilha *cel=(Pilha*)malloc(sizeof(Pilha));
    cel->n=v;
    cel->prox=*p;
    *p=cel;
}

void pop(Pilha **p){
    Pilha *aux;
    aux=*p;
    if(*p == NULL){
        *p=(*p)->prox;
        free(aux);
    }
}

void copiaPilha(Pilha **p1,Pilha **p2){
    Pilha *aux=(Pilha*)malloc(sizeof(Pilha));
    if(*p1 == NULL){
        *p2 = NULL;
    }else{
        copiaPilha(&(*p1)->prox,p2);
        aux=*p1;
        aux->prox=*p2;
        *p2 = aux;
    }
}

void LiberaP(Pilha **p){
    if(*p==NULL){
        *p=NULL;
    }else{
        LiberaP(&(*p)->prox);
        Pilha *aux=*p;
        *p=NULL;
        free(aux);
    }
}