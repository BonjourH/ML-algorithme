#include"softMax.c"
void main(){
//读文件及初始化
////////////////////////////////////////////////////////
////             【样本的格式】                      ////
////    3 4  数据维度，3个样本，自变量维度4-1=3       ////
////    1 2 4 0    sample1=[1,2,4], 属于第0类       ////
////    3 5 3 2    sample1=[3,5,3], 属于第2类       ////
////    5 9 6 1    sample1=[5,9,6], 属于第1类       ////
///////////////////////////////////////////////////////
FILE *fp=fopen("C:\\Users\\bigbe\\Documents\\Codeblock\\SoftMaxRegression_dataSet.txt","r");
Matrix *Input=(Matrix*)malloc(sizeof(Matrix));
Matrix *Output=(Matrix*)malloc(sizeof(Matrix));
DataSet_get(fp,Input,Output);
//原输入的转置,在梯度更新时会用到
Matrix *T_Input=mat_transpose(Input);
//【softMax分类初始化参数的时候最好不要为0，理由回忆实验出现的NAN】
Matrix *param=matrix_equal(Input->Nocolumn,KK,0);
//特殊矩阵，用于梯度更新及损失函数
Matrix *proba=mat_proba(Input,param);
Matrix *carac=mat_carac(Output,KK);
//损失函数   loss1是参数更新前的，loss2是参数更新后的
double loss1=0;
double loss2=softMax_Loss(carac,proba,param);
int i,j,iter;
double Ir=IR,tt=0.01;
//S是通过Loss函数不含正则部分对参数矩阵求导得到【要理解对矩阵求导数的意义】
Matrix *S;
for(iter=1;iter<50000&&fabs(loss1-loss2)>Threshold;iter++){//
    if(fabs(loss1-loss2)<tt){
        tt=tt/10;
        Ir=Ir/2;
        }
    S=mat_multipl(T_Input,mat_subtrac(carac,proba));
    //printf("\n");
    //【更新梯度】
    /////////////////////////////////////////////////////////////////////
    ///////这里更新参数时并没有计算损失函数，梯度并没有用新的参数更新 ////////
    ///////我们是在所有参数更新一次后再计算损失函数                 ////////
    /////////////////////////////////////////////////////////////////////
    for(i=0;i<param->Noraw;i++)
        for(j=0;j<param->Nocolumn;j++)
            param->cell[i][j]-=Ir*(-S->cell[i][j]/carac->Noraw+lamba*param->cell[i][j]);
    //更新proba矩阵与loss
    proba=mat_proba(Input,param);
    loss2=softMax_Loss(carac,proba,param);
    if(iter%50==0)
        printf("iter:%d loss:%lf\n",iter,loss2);
}
//写入数据
FILE *fp2=fopen("C:\\Users\\bigbe\\Documents\\Codeblock\\softMax_result.txt","w");
fprintf(fp2,"===============================================================================\n");
fprintf(fp2,"The input sample set is:\n");
for(i=0;i<Input->Noraw;i++){
    for(j=1;j<Input->Nocolumn;j++)
        fprintf(fp2,"%lf  ",Input->cell[i][j]);
    fprintf(fp2,"----->%lf  \n",Output->cell[i][0]);
    fprintf(fp2,"\n");
}
fprintf(fp2,"================================================================================\n");
fprintf(fp2,"The result:we get the probability of sample X belonging to class i.\n");
fprintf(fp2,"P(i|X)=exp(Wi.X+bi)/[exp(W0.X+b0)+exp(W1.X+b1)+...+exp(W(k-1).X+b(k-1))],i=0,1,2,...,k-1.\n");
fprintf(fp2,"Where W and b is:\n");
for(i=0;i<param->Nocolumn;i++){
    fprintf(fp2,"W%d: [ ",i);
    for(j=1;j<param->Noraw;j++)
        fprintf(fp2,"%lf ",param->cell[j][i]);
    fprintf(fp2,"], b%d=%lf\n",i,param->cell[0][i]);
}
fprintf(fp2,"================================================================================\n");
fprintf(fp2,"And we get probability matrix by using the classfication function,\n");
fprintf(fp2,"where the P[i,j] represent the probability of ith sampel belonging to class j.\n");
for(i=0;i<Input->Noraw;i++){
    for(j=0;j<KK;j++)
        fprintf(fp2,"%lf ",proba->cell[i][j]);
    fprintf(fp2,"\n");
}
fprintf(fp2,"================================================================================\n");
fprintf(fp2,"To evaluate our result, we can see the carac matrix,");
fprintf(fp2,"where p[i][j]=1 represent that the ith sample belong to j class.\n");
for(i=0;i<Input->Noraw;i++){
    for(j=0;j<KK;j++)
        fprintf(fp2,"%lf ",carac->cell[i][j]);
    fprintf(fp2,"\n");
}
fprintf(fp2,"================================================================================\n");
fclose(fp2);
//free内存
Free_mat(Input);
Free_mat(Output);
Free_mat(T_Input);
Free_mat(param);
Free_mat(proba);
Free_mat(carac);
Free_mat(S);
fclose(fp);
}

