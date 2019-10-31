#include<stdio.h>
#include<stdlib.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
};

typedef struct grafos Grafos;

struct torre{
    Grafos *gr;
    int estado[81][4];
    int nivel[81];
};

struct pilha{
    int n;
    struct pilha *prox;
};

struct maior{
    int tam;
    struct pilha *caminho;
};


typedef struct pilha Pilha;
typedef struct maior Maior;
typedef struct torre Torre;

Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);
void buscaProf(Grafos *gr, int raiz, int *visitado, Maior *cam);
void buscaEmProfundidade(Grafos *gr, int raiz, int *visitado, Pilha **caminho, int cont, Maior *cam);
void push(int v, Pilha **p);
void pop(Pilha **p);
void copiaPilha(Pilha **p1,Pilha **p2);
void mostraCam(Pilha *caminho);
void LiberaP(Pilha **p);
void conectar(Torre *hanoi);
int move1Disco(Torre *hanoi, int origem, int destino, int *pos);
int podeBotar(Torre *hanoi, int destino, int pos);
void mostraEstados(Torre *hanoi);

int main(){
    int i, j, k, l;
    Torre *hanoi; 
    //3^n => 3 ^ 4 == 81
    //Grafos *gr;
    hanoi->gr = criaGrafo(81, 0);
    Maior *cam=(Maior*)malloc(sizeof(Maior));
    cam->tam=0;
    cam->caminho=NULL;
    int contPos = 0;
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            for(k=0; k<3; k++){
                for(l=0; l<3; l++){
                    hanoi->estado[contPos][i] = i+1;
                    hanoi->estado[contPos][j] = j+1;
                    hanoi->estado[contPos][k] = k+1;
                    hanoi->estado[contPos][l] = l+1;
                    contPos++;
                }
            }
        }
    }
    contPos = 0;
    /*for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            for(k=0; k<3; k++){
                for(l=0; l<3; l++){
                    printf("%d, %d, %d, %d\n", hanoi->estado[contPos][i], hanoi->estado[contPos][j], hanoi->estado[contPos][k], hanoi->estado[contPos][l] = l+1);
                    contPos++;
                }
            }
        }
    }*/
    conectar(hanoi);
    mostraEstados(hanoi);
    return 0;
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

int move1Disco(Torre *hanoi, int origem, int destino, int *pos){
    int i, cont = 0;
    for(i=0; i<4; i++){
        if(hanoi->estado[origem][i] != hanoi->estado[destino][i]){
            *pos = i;
            cont++;
        }
    }
    if(cont != 1){
        cont = 0;
    }
    return cont;
}

int podeBotar(Torre *hanoi, int destino, int pos){
    int i, result = 1;
    for(i=pos+1; i<4; i++){
        if(hanoi->estado[destino][pos] == hanoi->estado[destino][i]){
            result = 0;
        }
    }
    return result;
}

void conectar(Torre *hanoi){
    int i, j, pos;
    for(i=0; i<81; i++){
        for(j=i; j<81; j++){
            if(move1Disco(hanoi, i, j, &pos)){
                if(podeBotar(hanoi, i, pos)){
                    if(podeBotar(hanoi, j, pos)){
                        insereAresta(&hanoi->gr, i, j, 0, 0);
                    }
                }
            }
        }
    }
}

void mostraEstados(Torre *hanoi){
    int i, j, k, l, contPos = 0;
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            for(k=0; k<3; k++){
                for(l=0; l<3; l++){
                    printf("%d: %d, %d, %d, %d\n",contPos, hanoi->estado[contPos][i], hanoi->estado[contPos][j], hanoi->estado[contPos][k], hanoi->estado[contPos][l] = l+1);
                    contPos++;
                }
            }
        }
    }
     for(i=0; i<hanoi->gr->nVertices; i++){
         printf("%d: ", i);
        for(j=0; j<hanoi->gr->grau[i]; j++){
            printf("%d ", hanoi->gr->arestas[i][j]);
        }
        printf("\n");
     }
}

void buscaEmProfundidade(Grafos *gr, int raiz, int *visitado, Pilha **caminho, int cont, Maior *cam){
    int i;
    visitado[raiz] = -1;
    push(raiz,caminho);
    for(i=0; i<gr->grau[raiz]; i++){
        if(!visitado[gr->arestas[raiz][i]]){
            buscaEmProfundidade(gr, gr->arestas[raiz][i], visitado, caminho, cont+1, cam);
        }
    }
        if(cont>cam->tam){
            LiberaP(&cam->caminho);
            cam->caminho=NULL;
            copiaPilha(caminho,&cam->caminho);
            cam->tam=cont;
        }
    pop(caminho);
    visitado[raiz]=0;
}


void buscaProf(Grafos *gr, int raiz, int *visitado, Maior *cam){
    int i, cont = 1;
    for(i=0; i<gr->nVertices; i++){
        visitado[i] = 0;
    }
    Pilha *Caminho=(Pilha*)malloc(sizeof(Pilha));
    Caminho=NULL;
    cont=0;
    buscaEmProfundidade(gr, raiz, visitado, &Caminho, cont, cam);
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
    if(*p != NULL){
        (*p)=(*p)->prox;
        free(aux);
        aux = NULL;
    }
}

void copiaPilha(Pilha **p1,Pilha **p2){
    Pilha *aux=(Pilha*)malloc(sizeof(Pilha));
    if(*p1 == NULL){
        *p2 = NULL;
    }else{
        copiaPilha(&(*p1)->prox,p2);
        aux->n = (*p1)->n;
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
