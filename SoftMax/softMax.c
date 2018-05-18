#include"softMax.h"
//返回m*k大小的概率矩阵，p[i][j]为第(i+1)个样本属于第j类的概率
//注意这里的陷阱以及softMax函数的特征。
//问题:参数param如果是负（正）数而且绝对值很大，那么计算概率时分母会接近0（正无穷），这时0/0（无穷/无穷），电脑不能判别
//为了解决这个问题，我们采用param的一个性质，即所有参数同时加上一个值，将分母调到合适的值而又不影响概率的计算
Matrix* mat_proba(Matrix* Input,Matrix *Param){
    Matrix *param=softmax_param(Param);
    int i,j;
    double somme=0;
    double **p=(double**)malloc(sizeof(double)*Input->Noraw);
    for(i=0;i<Input->Noraw;i++){
        p[i]=(double *)malloc(sizeof(double)*param->Nocolumn);
        for(j=0;j<param->Nocolumn;j++){
            somme+=expf(dot_product(Input,i+1,param,j+1));//【特别注意这里加1了，看函数原型】
        }
        //printf("somme=%lf\n",somme);
        for(j=0;j<param->Nocolumn;j++){
            p[i][j]=expf(dot_product(Input,i+1,param,j+1))/somme;
        }
        somme=0;
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->Noraw=Input->Noraw;
    q->Nocolumn=param->Nocolumn;
    q->cell=p;
    //show_mat(q);
    return q;
}
//解决上面的浮点数越界问题
Matrix* softmax_param(Matrix *M){
    double min=getMin_mat(M);
    return mat_add(M,matrix_equal(M->Noraw,M->Nocolumn,-min));
}
//返回m*k大小矩阵，m为样本个数，k为类别数
Matrix* mat_carac(Matrix* Output,int k){
    int i,j;
    double **p=(double**)malloc(sizeof(double)*Output->Noraw);
    for(i=0;i<Output->Noraw;i++){
        p[i]=(double *)malloc(sizeof(double)*k);
        for(j=0;j<k;j++)
            p[i][j]=Output->cell[i][0]==j?1:0;
    }
    Matrix *q=(Matrix*)malloc(sizeof(Matrix));
    q->Noraw=Output->Noraw;
    q->Nocolumn=k;
    q->cell=p;
    return q;
}
//返回分类的loss
double softMax_Loss(Matrix *carac,Matrix *proba,Matrix *param){
    //【问题是log_mat的时候会出现log0的情况】
   // return lamba*MatNorm(param)/2-somme_mat(Hadamard_mat_product(carac,log_mat(proba)))/carac->Noraw;
    double som=0;
    int i,j;
    for(i=0;i<carac->Noraw;i++){
        for(j=0;j<carac->Nocolumn;j++){
            if(carac->cell[i][j]!=0)//本质上是0*log0=0,但是计算机无法判断
                som+=log(proba->cell[i][j]);
        }
    }
    return lamba*MatNorm(param)/2-som/carac->Noraw;
};
