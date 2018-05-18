#include <math.h>
#include "Matrix.h"
////////////////////////////////////////////
void DataSet_get(FILE *fp,Matrix *input,Matrix *output){
    if(fp==NULL){
        printf("file open error\n");
        exit(0);
    }
    //读矩阵的维度
    /////////////////////////////////////////////////////
    /////这里的函数对txt文档的编辑有一定要求:       ///////
    /////第一行为m n，两个数代表txt文档数据的行与列 ///////
    /////例如： 2 5                             ///////
    /////      1 2 3 4 6                       //////
    /////      2 6 7 4 5                      //////
    ///////////////////////////////////////////////
    int raw,column;
    fscanf(fp,"%d",&raw);
    fscanf(fp,"%d",&column);
    //将输入{X1,X2,...,Xm}读入input，输出{y1,y2,...,ym}读入output
    double **p=(double**)malloc(sizeof(double)*raw);
    double **q=(double**)malloc(sizeof(double)*raw);
    int i,j;
    for(i=0;i<raw;i++){
        //要注意矩阵的形式,第一列的1原本文件中并没有。
        //input=|1,x11,x12,...,x1n|
        //      |1,x21,x22,...,x2n|
        //      |.................|
        //      |.................|
        //      |1,xm1,xm2,...,xmn|
        p[i]=(double *)malloc(sizeof(double)*column);
        q[i]=(double *)malloc(sizeof(double)*1);
        p[i][0]=1;
        for(j=1;j<column;j++)
            fscanf(fp,"%lf",&p[i][j]);
        fscanf(fp,"%lf",&q[i][0]);
    }
    printf("=============================================\n");
    printf("The input sample set is:\n");
    for(i=0;i<raw;i++){
        for(j=1;j<column;j++)
                printf("%lf  ",p[i][j]);
        printf("-----%lf\n",q[i][0]);
        printf("\n");
    }
    printf("=============================================\n");
    input->Noraw=raw;
    input->Nocolumn=column;
    input->cell=p;
    output->Nocolumn=1;
    output->Noraw=raw;
    output->cell=q;
    fclose(fp);
}
////////////////////////////////////////////
Matrix* matrix_creat(int raw,int column){
    double **p=(double**)malloc(sizeof(double)*raw);
    int i,j;
    for(i=0;i<raw;i++){
        p[i]=(double *)malloc(sizeof(double)*column);
        for(j=0;j<column;j++)
            p[i][j]=0;
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->Noraw=raw;
    q->Nocolumn=column;
    q->cell=p;
    return q;
}
////////////////////////////////////////////
Matrix* matrix_equal(int raw,int column,double k){
    double **p=(double**)malloc(sizeof(double)*raw);
    int i,j;
    for(i=0;i<raw;i++){
        p[i]=(double *)malloc(sizeof(double)*column);
        for(j=0;j<column;j++)
            p[i][j]=k;
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->Noraw=raw;
    q->Nocolumn=column;
    q->cell=p;
    return q;
}
////////////////////////////////////////////
Matrix* mat_multipl(Matrix* A,Matrix* B){
    if(A->Nocolumn!=B->Noraw){
        printf("矩阵维度不匹配\n");
        return;
    }
    double **p=(double**)malloc(sizeof(double)*A->Noraw);
    int i,j,k;
    for(i=0;i<A->Noraw;i++){
        p[i]=(double *)malloc(sizeof(double)*B->Nocolumn);
        for(j=0;j<B->Nocolumn;j++){
            p[i][j]=0;
            for(k=0;k<A->Nocolumn;k++)
                p[i][j]+=A->cell[i][k]*B->cell[k][j];
        }
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->cell=p;
    q->Nocolumn=B->Nocolumn;
    q->Noraw=A->Noraw;
    return q;
}
/////////////////////////////////////////////
Matrix* mat_add(Matrix* A,Matrix* B){
    if((A->Nocolumn!=B->Nocolumn)&&(A->Noraw!=B->Noraw)){
        printf("矩阵维度不匹配\n");
        return;
    }
    double **p=(double**)malloc(sizeof(double)*A->Noraw);
    int i,j,k;
    for(i=0;i<A->Noraw;i++){
        p[i]=(double *)malloc(sizeof(double)*A->Nocolumn);
        for(j=0;j<A->Nocolumn;j++)
            p[i][j]=A->cell[i][j]+B->cell[i][j];
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->cell=p;
    q->Nocolumn=A->Nocolumn;
    q->Noraw=A->Noraw;
    return q;
}
/////////////////////////////////////////////
Matrix* mat_subtrac(Matrix* A,Matrix* B){
    if((A->Nocolumn!=B->Nocolumn)&&(A->Noraw!=B->Noraw)){
        printf("矩阵维度不匹配\n");
        return;
    }
    double **p=(double**)malloc(sizeof(double)*A->Noraw);
    int i,j,k;
    for(i=0;i<A->Noraw;i++){
        p[i]=(double *)malloc(sizeof(double)*A->Nocolumn);
        for(j=0;j<A->Nocolumn;j++)
            p[i][j]=A->cell[i][j]-B->cell[i][j];
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->cell=p;
    q->Nocolumn=A->Nocolumn;
    q->Noraw=A->Noraw;
    return q;
}
////////////////////////////////////////////
Matrix* mat_transpose(Matrix* A){
    int raw=A->Nocolumn;
    int column=A->Noraw;
    double **p=(double**)malloc(sizeof(double)*A->Nocolumn);
    int i,j;
    for(i=0;i<raw;i++){
        p[i]=(double *)malloc(sizeof(double)*A->Noraw);
        for(j=0;j<column;j++)
            p[i][j]=A->cell[j][i];
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->Nocolumn=column;
    q->Noraw=raw;
    q->cell=p;
    return q;
}

////////////////////////////////////////////
double somme_mat(Matrix *M){
    int i,j;
    double som=0;
    for(i=0;i<M->Noraw;i++)
        for(j=0;j<M->Nocolumn;j++)
            som+=M->cell[i][j];
    return som;
}
////////////////////////////////////////////
double dot_product(Matrix *A,int rawA,Matrix *B,int columnB){
    if(A->Nocolumn!=B->Noraw){
        printf("The matrix dimension dosen't match.\n");
        return;
    }
    int i;
    double product=0;
    for(i=0;i<A->Nocolumn;i++)
        product+=A->cell[rawA-1][i]*B->cell[i][columnB-1];
    return product;
}
////////////////////////////////////////////
double MatNorm(Matrix *M){
    double norm=0;
    int i,j;
    for(i=0;i<M->Noraw;i++)
        for(j=0;j<M->Nocolumn;j++)
            norm+=M->cell[i][j]*M->cell[i][j];
    return sqrt(norm);
}
////////////////////////////////////////////
Matrix* log_mat(Matrix *A){
    double **p=(double**)malloc(sizeof(double)*A->Noraw);
    int i,j;
    for(i=0;i<A->Noraw;i++){
        p[i]=(double *)malloc(sizeof(double)*A->Nocolumn);
        for(j=0;j<A->Nocolumn;j++)
            p[i][j]=log(A->cell[i][j]);
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->cell=p;
    q->Nocolumn=A->Nocolumn;
    q->Noraw=A->Noraw;
    return q;
};
////////////////////////////////////////////
void show_mat(Matrix *M){
    int i,j;

    for(i=0;i<M->Noraw;i++){
        for(j=0;j<M->Nocolumn;j++)
            printf("%lf\t",M->cell[i][j]);
        printf("\n");
    }
    printf("----------------------------\n");
}
////////////////////////////////////////////
void Free_mat(Matrix *M){
    int i;
    for(i=0;i<M->Noraw;i++)
        free(M->cell[i]);
    free(M);
}
////////////////////////////////////////////
Matrix* Hadamard_mat_product(Matrix *A,Matrix *B){
    if((A->Nocolumn!=B->Nocolumn)&&(A->Noraw!=B->Noraw)){
        printf("矩阵维度不匹配\n");
        return;
    }
    double **p=(double**)malloc(sizeof(double)*A->Noraw);
    int i,j,k;
    for(i=0;i<A->Noraw;i++){
        p[i]=(double *)malloc(sizeof(double)*A->Nocolumn);
        for(j=0;j<A->Nocolumn;j++)
            p[i][j]=A->cell[i][j]*B->cell[i][j];
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->cell=p;
    q->Nocolumn=A->Nocolumn;
    q->Noraw=A->Noraw;
    return q;
}
double getMin_mat(Matrix *M){
    double min=M->cell[0][0];
    int i,j;
    for(i=0;i<M->Noraw;i++){
        for(j=0;j<M->Nocolumn;j++){
            if(min>M->cell[i][j])
                min=M->cell[i][j];
        }
    }
    return min;
}
/*
////////////////////////////////////////////
Vector* vector_get(float *a){
    int length=0;
    for(length=0;*(a+length)!='\0';length++)
    Vector *q;
    q->length=length;
    q->cor=a;
    return q;
}
////////////////////////////////////////////
Vector* vector_creat(int n){
    float *p=(int*)malloc(sizeof(float)*n);
    Vector *q;
    q->cor=p;
    inti;
    for(i=0;i<n;i++)
        q->cor[i]=0;
    q->length=n;
    return q;
}
////////////////////////////////////////////
Vector* vec_subtrac(Vector* A,Vector* B){
    if(A->length!=B->length){
        printf("向量维度不匹配\n");
        return;
    }
    float *p=(int*)malloc(sizeof(float)*(A->length));
    int i;
    for(i=0;i<A->length;i++)
        p[i]=A->cor[i]-B->cor[i];
    Vector *q;
    q->length=A->length;
    q->cor=p;
    return q;
}
////////////////////////////////////////////
float eucliNorm(Vector *V){
    float norm=0;
    int i;
    for(i=0;i<V->length;i++)
        norm+=V->cor[i]*V->cor[i];
    return sqrt(norm);
}
*/
