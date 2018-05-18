#include <stdio.h>
#include <stdlib.h>
struct matrix{
    int Noraw;
    int Nocolumn;
    double **cell;
};
typedef struct matrix Matrix;
void DataSet_get(FILE *fp,Matrix *input,Matrix *output);//从txt文件中读入数据集
Matrix* matrix_creat(int raw,int column);//初始化raw*column的0矩阵
Matrix* matrix_equal(int raw,int column,double k);//初始化raw*column的矩阵，每个元素为k
Matrix* mat_multipl(Matrix* A,Matrix* B);//A*B
Matrix* Hadamard_mat_product(Matrix *A,Matrix *B);//矩阵阿达马乘积(也称分素乘积entrywise product)
Matrix* mat_add(Matrix* A,Matrix* B);//A+B
Matrix* mat_subtrac(Matrix* A,Matrix* B);//A-B
Matrix* mat_transpose(Matrix* A);//转置
Matrix* log_mat(Matrix *A);//返回对数矩阵，一般用于求对数概率

double MatNorm(Matrix *M); //求矩阵的第二范数，这里在后面用于求损失函数的L2正则项
double dot_product(Matrix *A,int rawA,Matrix *B,int columnB);//A矩阵第rawA行与B矩阵columnB相乘
double somme_mat(Matrix *M);//将矩阵中所有元素加起来
double getMin_mat(Matrix *M);//返回矩阵最小值
void show_mat(Matrix *M);//输出矩阵
void Free_mat(Matrix *M);//释放M矩阵的内存
/*
typedef struct vector Vector;
struct vector{
    int length;
    float *cor;
}
Vector* vec_subtrac(Vector* A,Vector* B);
Vector* vector_get(float *a);
Vector* vector_creat(int n);
float eucliNorm(Vector *V);
*/
