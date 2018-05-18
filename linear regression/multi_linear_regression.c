/////////////////////////////////////////////////////////////////////////////
//Input：   数据集 S={s1,s2,...,sm}   ith个样本si=（x1,x2,x3,...,xn,y）  /////
//          其中X=（x1,...,xn)是自变量，y属于因变量                      /////
//Output：  n维线性回归函数y=<a0,a1,a2,...,an>.<1,x1,x2,...,xn>,.是内积  ////
//////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//Two method：                                       //////////
//          1：由于损失函数为凸函数，可用梯度下降      //////////
//          2：利用法方程求解出参数theta的解析表达式  //////////
////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
//////【注意 LineraRegression_dataSet.txt格式】       /////////
////// 例如：                                        /////////
//////      2 3             第一行两个数指明数据维度  /////////
//////      1 2 3           X1={1,2} y1=3          /////////
//////      2 4 6           X2={2,4} y2=6         /////////
//////////////////////////////////////////////////////////
#include"Matrix.c"
#define IR 0.01          //基础学习率
#define step 1000        //每隔step次循环学习率下降为原来的1/2
#define Threshold 0.00001//Loss函数的阈值，当Loss变化小于此值时停止循环迭代
#define Maxiter 15000    //梯度更新的最大循环次数
#define lamba 0.000001   //正则参数
void main(){
//读文件及初始化
FILE *fp=fopen("C:\\Users\\bigbe\\Documents\\Codeblock\\LineraRegression_dataSet.txt","r");
Matrix *Input=(Matrix*)malloc(sizeof(Matrix));
Matrix *Output=(Matrix*)malloc(sizeof(Matrix));
DataSet_get(fp,Input,Output);
//原输入的转置
Matrix *T_Input=mat_transpose(Input);
//参数
Matrix *param=matrix_creat(Input->Nocolumn,1);
//Delta=Input*param-Output
Matrix *Delta=mat_subtrac(mat_multipl(Input,param),Output);
//损失函数   loss1是参数更新前的，loss2是参数更新后的
double loss1=0;
double loss2=MatNorm(Delta)+lamba*MatNorm(param);
//【梯度下降更新参数】
////////////////////////////////////////////////////////////////
/////几个问题：                                      ///////////
/////          1：是一次更新所有参数还是及时更新      ///////////
/////         2：参数是一个一个更新还是所有过一遍    ///////////
////////////////////////////////////////////////////////////
/////        经过试验，即时更新比一次快10倍(运算时间) ////////
/////        经过相同次数迭代，即时更新优化更好      ////////
/////////////////////////////////////////////////////////
int i,j,iter;
double Ir=IR;

//【1：即时更新策略，thetaj更新之后会利用新的thetaj重新计算损失函数】//
double S;
for(iter=1;iter<Maxiter;iter++){//&&(fabs(loss1-loss2)>0.00001

    loss1=loss2;
    if(iter%step==0&&Ir>0.000002)//不能让学习率过低
            Ir=Ir/5;
    for(i=0;i<param->Noraw;i++){
        S=dot_product(T_Input,i+1,Delta,1);
        param->cell[i][0]-=Ir*(S/Input->Noraw+lamba*param->cell[i][0]);
        Delta=mat_subtrac(mat_multipl(Input,param),Output);
    }
    loss2=MatNorm(Delta)+lamba*MatNorm(param);
    if(iter%20==0)
        printf("iter:%d  loss2:%lf\n",iter,loss2);
}
    //写入数据
FILE *fp2=fopen("C:\\Users\\bigbe\\Documents\\Codeblock\\LR_result1.txt","w");
fprintf(fp2,"=============================================\n");
fprintf(fp2,"The input sample set is:\n");
for(i=0;i<Input->Noraw;i++){
    for(j=1;j<Input->Nocolumn;j++)
        fprintf(fp2,"%lf  ",Input->cell[i][j]);
    fprintf(fp2,"----->%lf  \n",Output->cell[i][0]);
    fprintf(fp2,"\n");
}
fprintf(fp2,"=============================================\n");
fprintf(fp2,"The linear appromative function is:\ny=%lf",param->cell[0][0]);
for(i=1;i<param->Noraw;i++){
    if(param->cell[i][0]<0)
        fprintf(fp2,"%lf*x%d",param->cell[i][0],i);
    else
        fprintf(fp2,"+%lf*x%d",param->cell[i][0],i);
}
fprintf(fp2,"\n");
fprintf(fp2,"=============================================\n");
fprintf(fp2,"The predict result is:\n");
for(i=0;i<Input->Noraw;i++)
    fprintf(fp2,"%lf\n",mat_multipl(Input,param)->cell[i][0]);
fprintf(fp2,"=============================================\n");
fclose(fp2);
/*
//【2：一次更新所有参数，thetaj更新之后不重新计算损失函数，到所有的参数更新一遍之后再计算损失函数】//
Matrix *SS;
for(iter=0;iter<Maxiter;iter++){//&&fabs(loss1-loss2)>Threshold
    loss1=loss2;
    if(iter%step==0&&Ir>0.000002)
        Ir=Ir/2;
    SS=mat_multipl(T_Input,Delta);
    for(i=0;i<param->Noraw;i++)
        param->cell[i][0]-=Ir*(SS->cell[i][0]/Input->Noraw+lamba*param->cell[i][0]);
    loss2=MatNorm(Delta)+lamba*MatNorm(param);
    Delta=mat_subtrac(mat_multipl(Input,param),Output);
    if(iter%20==0)
        printf("iter:%d  loss2:%lf\n",iter,loss2);
}
FILE *fp3=fopen("C:\\Users\\bigbe\\Documents\\Codeblock\\LR_result2.txt","w");
fprintf(fp3,"=============================================\n");
fprintf(fp3,"The input sample set is:\n");
for(i=0;i<Input->Noraw;i++){
    for(j=1;j<Input->Nocolumn;j++)
        fprintf(fp3,"%lf  ",Input->cell[i][j]);
    fprintf(fp3,"----->%lf\n",Output->cell[i][0]);
    fprintf(fp3,"\n");
}
fprintf(fp3,"=============================================\n");
fprintf(fp3,"The linear appromative function is:\ny=%lf",param->cell[0][0]);
for(i=1;i<param->Noraw;i++){
    if(param->cell[i][0]<0)
        fprintf(fp3,"%lf*x%d",param->cell[i][0],i);
    else
        fprintf(fp3,"+%lf*x%d",param->cell[i][0],i);
}
fprintf(fp3,"\n");
fprintf(fp3,"=============================================\n");
fprintf(fp3,"The predict result is:\n");
for(i=0;i<Input->Noraw;i++)
    fprintf(fp3,"%lf\n",mat_multipl(Input,param)->cell[i][0]);
fprintf(fp3,"=============================================\n");
fclose(fp3);
*/
//释放内存
Free_mat(Input);
Free_mat(Output);
Free_mat(T_Input);
Free_mat(param);
Free_mat(Delta);
//Free_mat(SS->cell);
}
