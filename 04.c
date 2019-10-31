#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct grafos{
    int nVertices, ehPonderado, *grau;
    int **arestas, **pesos;
};

struct pilha{
    int n;
    struct pilha *prox;
};

struct maior{
    int tam;
    struct pilha *caminho;
};

typedef struct grafos Grafos;
typedef struct pilha Pilha;
typedef struct maior Maior;

Grafos* criaGrafo(int vertice, int ehPonderado);
void insereAresta(Grafos **gr, int origem, int destino, int peso, int ehDigrafo);
void buscaLarg(Grafos *gr, int raiz, int *visitado, Maior **cam, int money);
void buscaEmLargura(Grafos *gr, int raiz, int *visitado, Pilha **caminho, int cont, Maior **cam, int money);
void push(int v, Pilha **p);
void pop(Pilha **p);
void copiaPilha(Pilha **p1,Pilha **p2);
void mostraCam(Pilha *caminho);
void LiberaP(Pilha **p);
int **ordena(int **mat, int cont);
int naoEhAdjacente(Grafos *gr, int vertice, int check);

int main(){
    Grafos *gr;
    Maior *cam=(Maior*)malloc(sizeof(Maior));
    cam->tam=0;
    cam->caminho=NULL;
    int i, j, money, cidades, rota, pedagio, origem;
    printf("Digite a quantidade de dinheiro disponivel:\n");
    scanf("%d", &money);
    printf("Digite a quantidade cidades:\n");
    scanf("%d", &cidades);
    int vis[cidades];
    gr = criaGrafo(cidades, 1);
    for(i=0; i<cidades; i++){
        for(j=0; j<cidades; j++){
            //Fazer com que quando escolha ij não pergunte ji.
            if(i != j && naoEhAdjacente(gr, j, i)){
                printf("Existe uma estrada entre a cidade %d e a cidade %d?[1-Sim/0-Não]:\n", i, j);
                scanf("%d", &rota);
                if(rota == 1){
                    printf("Digite o valor do pedagio dessa estrada:\n");
                    scanf("%d", &pedagio);
                    insereAresta(&(gr), i, j, pedagio, 0);
                }
            }
        }
    }
    printf("Digite a cidade de origem:\n");
    scanf("%d", &origem);
    buscaLarg(gr, origem, vis, &cam, money);
    mostraCam(cam->caminho);
    return 0;
}

int naoEhAdjacente(Grafos *gr, int vertice, int check){
    int i, result = 1;
    for(i=0; i<gr->grau[vertice]; i++){
        if(gr->arestas[vertice][i] == check){
            result = 0;
        }
    }
    return result;
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


int **ordena(int **mat, int cont){
    int i, it = 0, contMenos1 = 0, indice = 0, **visita = (int**) malloc(sizeof(int*)*2);
    float valor;
    for(i=0; i<2; i++){
        visita[i] = (int*) malloc(sizeof(int)*cont);
    }
    while(contMenos1 != cont){
        contMenos1 = 0;
        valor = INFINITY;
        for(i=0; i<cont; i++){
            if(mat[1][i] < valor && mat[1][i] != -1){
                valor = mat[1][i];
                indice = i;
            }else if(mat[1][i] == -1){
                contMenos1++;
            }
        }
        if(contMenos1 != cont){
            visita[0][it] = mat[0][indice];
            visita[1][it] = mat[1][indice];
            mat[1][indice] = -1;
            it++;
        }
    }
    return visita;
}

void buscaEmLargura(Grafos *gr, int raiz, int *visitado, Pilha **caminho, int cont, Maior **cam, int money){
    int i, **vaiVisitar, contVisita = 0;
    vaiVisitar = (int**) malloc(sizeof(int*)*2);
    for(i=0; i<2; i++){
        vaiVisitar[i] = (int*) malloc(sizeof(int)*gr->grau[raiz]);
    }
    visitado[raiz] = -1;
    push(raiz,caminho);
    for(i=0; i<gr->grau[raiz]; i++){
        if(!visitado[gr->arestas[raiz][i]] && (money - gr->pesos[raiz][i])>=0){
            vaiVisitar[0][contVisita] = gr->arestas[raiz][i];
            vaiVisitar[1][contVisita] = gr->pesos[raiz][i];
            contVisita++;
        }
    }
    int **mat;
    mat = ordena(vaiVisitar, contVisita);
    for(i=0; i<contVisita; i++){
        buscaEmLargura(gr, mat[0][i], visitado, caminho, cont+1, cam, (money - mat[1][i]));
    }
        if(cont>(*cam)->tam){
            LiberaP(&(*cam)->caminho);
            (*cam)->caminho=NULL;
            copiaPilha(caminho,&(*cam)->caminho);
            (*cam)->tam=cont;
        }
    pop(caminho);
    visitado[raiz]=0;
}

void buscaLarg(Grafos *gr, int raiz, int *visitado, Maior **cam, int money){
    int i, cont = 1;
    for(i=0; i<gr->nVertices; i++){
        visitado[i] = 0;
    }
    Pilha *Caminho=(Pilha*)malloc(sizeof(Pilha));
    Caminho=NULL;
    cont=0;
    buscaEmLargura(gr, raiz, visitado, &Caminho, cont, cam, money);
}