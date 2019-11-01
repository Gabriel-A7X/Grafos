#include<stdio.h>
#include<stdlib.h>

typedef struct{
    int **Mde4,**Mde3,**Mde2,**Mde1;
}Estados;


int** troca(int **mat,int ii,int jj){
    for(int i=0;i<ii;i++){
        for(int j=0;j<jj;j++){
            if(mat[i][j]==0) mat[i][j]= -1;
        }
    }
    return mat;
}

Estados *inicializaEst(){
    Estados *est=(Estados*)malloc(sizeof(Estados));
    int **mat,**mat2,**mat3,*mat4;
    mat=(int**)malloc(sizeof(int*)*16);
    for(int i=0;i<16;i++){
        mat[i]=(int*)malloc(sizeof(int*)*4);
    }
    mat2=(int**)malloc(sizeof(int*)*8);
    for(int i=0;i<16;i++){
        mat2[i]=(int*)malloc(sizeof(int*)*3);
    }
    mat3=(int**)malloc(sizeof(int*)*4);
    for(int i=0;i<16;i++){
        mat3[i]=(int*)malloc(sizeof(int*)*2);
    }
    mat4=(int*)malloc(sizeof(int*)*2);
    int aa=1,bb=1,cc=1,dd=1;
    for (int i = 0; i < 16; ++i) {
        if (!(i % 8)) aa = !aa;
        if (!(i % 4)) bb = !bb;
        if (!(i % 2)) cc = !cc; 
        dd = !dd;
        mat[i][0]=aa;
        mat[i][1]=bb;
        mat[i][2]=cc;
        mat[i][3]=dd;
    }
    bb=1,cc=1,dd=1;
    for (int i = 0; i < 8; ++i) {
        if (!(i % 4)) bb = !bb;
        if (!(i % 2)) cc = !cc; 
        dd = !dd;
        mat2[i][0]=bb;
        mat2[i][1]=cc;
        mat2[i][2]=dd;
    }
    cc=1,dd=1;
    for (int i = 0; i < 4; ++i) {
        if (!(i % 2)) cc = !cc; 
        dd = !dd;
        mat3[i][0]=cc;
        mat3[i][1]=dd;
    }
    mat=troca(mat,16,4);
    mat2=troca(mat2,8,3);
    mat3=troca(mat3,4,2);
    mat4[0]=-1;
    mat4[1]=1;
    est->Mde4=mat;
    est->Mde3=mat2;
    est->Mde2=mat3;
    est->Mde1=(int**)malloc(sizeof(int*)*2);
    for(int i=0;i<2;i++){
        est->Mde1[i]=(int*)malloc(sizeof(int));
    }
    est->Mde1[0][0]=mat4[0];
    est->Mde1[1][0]=mat4[1];
    return est;
}

int main(){
    Estados *est=inicializaEst();

    for(int i=0;i<16;i++){
        printf("\t%d \t%d \t%d \t%d\n",est->Mde4[i][0],est->Mde4[i][1],est->Mde4[i][2],est->Mde4[i][3]);
    }
    for(int i=0;i<8;i++){
        printf("\t%d \t%d \t%d\n",est->Mde3[i][0],est->Mde3[i][1],est->Mde3[i][2]);
    }
    for(int i=0;i<4;i++){
        printf("\t%d \t%d\n",est->Mde2[i][0],est->Mde2[i][1]);
    }
    printf("\t%d\n",est->Mde1[0][0]);
    printf("\t%d\n",est->Mde1[1][0]);
}