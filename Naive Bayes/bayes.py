import numpy as np
import math

#目标：求出P(Y|X)的概率分布
#X={x1,x2,x3,...,xn} xi=0 或1， xi=1的代表包含i特征
#Y (- {1,2,3,...,k} Y为类别，我们处理的为K分类问题

###########################################
#【1】读取数据
#数据形式
#每一行：[x1,x2,...,xn, y]

#得到数据Data矩阵，以及标签label矩阵
data=[]
label=[]
#样本数量m，X维度n, 样本类别数k
m=10
n=10
k=10

##############################################
#【2】计算先验概率P(Y)以及条件概率P(xi|Y)的矩阵

#计算P(Y)
P_y=np.array([label.count(i+1)/m for i in range(k)])

#计算P(xi|Y)
'''
基于我们对特征X做出的条件独立性的假设，我们有
    P(X|Y)=P(x1|Y)*P(x2|Y)*...*P(xn|Y)
所以我们先计算P(xi|Y)的矩阵，得到P(xi|Y)，则易求P(X|Y)
Rq:需要注意的是xi可取两个值，这里我们计算P(xi=1|Y)的矩阵
'''
def P_xi_Y(data,label):
    P_xi_Y=np.zeros((k,n))
    for i in range(k):
        index = [i for i, v in enumerate(label) if (v == i + 1)]
        for j in range(n):
            #index为类别为i+1的所有数据索引列表
            num=0
            for ii in index:
                if(data[ii][j]==1):
                    num+=1
            P_xi_Y[i][j]=num/len(index)

#定义P(X|Y)计算函数
def P_X_Y(X,P,k):
    #X为数据，形式如[0,1,0,0,...,1]，X[i]=1表示X含有i+1特征
    #P为上面计算的矩阵
    #返回一个概率分布矩阵p=[p1,p2,...,pk]，pi为X属于i类别的概率
    p=np.zeros([1,k])
    for i in range(k):
        s=1
        for j in range(len(X)):
            s=s*(P[i][j]*X[j]+(1-P[i][j])*(1-X[j]))
        p[i]=s
    return p
#定义P(Y|X)概率计算函数
#参考贝叶斯公式
def P_Y_X(X,P_xi_Y,k,P_y):
    p=P_X_Y(X,P_xi_Y,k)
    numerator=p*P_y
    denominator=p.dot(P_y)
    return [i/denominator for i in numerator]
