import numpy as np
class bayes:
    def __init__(self,d,l,_m,_n,_k,_lambda=0):
        self.data=d
        self.label=l
        self.m=_m
        self.n=_n
        self.k=_k
        self.lamda=_lambda#涉及到贝叶斯估计，为1时为laplace smoothing
    def _P_Y(self):
        self.P_y=np.array([(self.label.count(i+1)+self.lamda)/(self.m+self.k*self.lamda) for i in range(self.k)])

    def _P_xi_Y(self):
        self.P_xi_Y = np.zeros((self.k, self.n))
        for i in range(self.k):
            index = [i for i, v in enumerate(self.label) if (v == i + 1)]
            for j in range(self.n):
                # index为类别为i+1的所有数据索引列表
                num = 0
                for ii in index:
                    if self.data[ii][j] == 1:
                        num += 1
                self.P_xi_Y[i][j] = (num+self.lamda) / (len(index)+2*self.lamda)
                # 定义P(X|Y)计算函数

    def _P_X_Y(self,X):
        # X为数据，形式如[0,1,0,0,...,1]，X[i]=1表示X含有i+1特征
        # P为上面计算的矩阵
        # 返回一个概率分布矩阵p=[p1,p2,...,pk]，pi为X属于i类别的概率
        self.P_X_Y = np.zeros([1, self.k])
        for i in range(self.k):
            s = 1
            for j in range(self.n):
                s = s * (self.P_xi_Y[i][j] * X[j] + (1 - self.P_xi_Y[i][j]) * (1 - X[j]))
            self.P_X_Y[i] = s
        return self.P_X_Y
    def P_Y_X(self,X):
        numerator = self._P_X_Y(X) * self.P_y
        denominator = self.P_X_Y.dot(self.P_y)
        return [i / denominator for i in numerator]

def main():
    #输入数据
    data=10
    label=10
    m=0
    n=0
    k=0
    #预测
    predict=bayes(data,label,m,n,k,0)
    X=[]#要验证的数据
    print("X属于给类的概率分别是：",predict.P_Y_X(X))