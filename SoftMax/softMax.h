//////////////////////////////////////////////////////////////////////////////
////////                   softMax实现一个K分类                          /////
//Input：   数据集 S={s1,s2,...,sm}   ith个样本si=（x1,x2,x3,...,xn,y）  /////
///////       其中X=（x1,...,xn)是自变量，y属于{0,2,...,k-1}             ////
//Output：  n维线性回归函数y=<a0,a1,a2,...,an>.<1,x1,x2,...,xn>,.是内积  ///
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////
////    method：  SGD     //////////
///////////////////////////////////
#include <stdio.h>
#include"Matrix.c"
#define IR 0.01          //学习率
#define step 1000        //每隔step次循环学习率下降为原来的1/10
#define lamba 0.0000     //正则参数
#define Threshold 0.000001//Loss函数的阈值，当Loss小于此值时停止循环迭代
#define Maxiter 10000    //梯度更新的最大循环次数
#define KK 2             //softMax分类数,当然从输入数据中判别会更好
Matrix* softmax_param(Matrix *M);//将矩阵所有值减去getMin_min(M)
Matrix* mat_proba(Matrix* Input,Matrix *param);//概率矩阵
Matrix* mat_carac(Matrix* Output,int k);//特征矩阵
double softMax_Loss(Matrix *carac,Matrix *proba,Matrix *param);//softMax的损失函数