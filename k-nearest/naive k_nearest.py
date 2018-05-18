import numpy as np
# naive version
class MaxHeap:
    def __init__(self):
        self.HeapList=[0]
        self.label=[]
        #为了配合K_nearest监督学习多设置的一项，建堆交换数据时也同时交换label
        self.heapsize=0
    def shiftDown(self,i):
        while 2*i<=self.heapsize:
            #判断有没有儿子
            if 2*i+1>self.heapsize:
                #如果没有右儿子
                j=2*i
            else:
                #如果有右儿子，比较两个儿子的大小，返回更小的索引
                if self.HeapList[2*i]<=self.HeapList[2*i+1]:
                    j=2*i
                else:
                    j=2*i+1
            if self.HeapList[i]>self.HeapList[j]:
                self.HeapList[j],self.HeapList[i]=self.HeapList[i],self.HeapList[j]
                self.label[i],self.label[j]=self.label[j],self.label[i]

    def bulidHeap(self,List,label):
        self.label=label
        self.HeapList=[0]+List[:]
        self.heapsize=len(List)
        i=self.heapsize//2
        #Rq：注意这里的i取值，仔细观察二叉树堆，可以发现<=i的都有儿子但是大于i+1的都没有儿子
        while i>0:
            self.shiftDown(i)
            i-=1

class k_nerest(MaxHeap):
    def __init__(self,_k=0,_C=0,_data=[],_label=[]):
        super().__init__()
        self.K=_k #K个近邻
        self.C=_C #C个类别
        self.data=_data
        self.label=_label
    def distance(self,a,b):
        #我们取L2范数
        a=np.array(a)
        b=np.array(b)
        return np.sqrt((a-b).dot(a-b))
    #方法1:线性扫描
    def linear_scan(self,X):
        #找到距离矩阵
        dis=[self.distance(X,i) for i in self.data]
        Min=min(dis)-1
        index=[]
        for _ in range(self.K):
            Max=max(dis)
            index_=dis.index(Max)
            index.append(index_)
            dis[index_]=Min
        #至此，index变为dis中最大k个数的索引
        #复杂度k*n
        Class=[self.label[index[i]] for i in range(len(index))]
        time=[Class.count(i) for i in range(1,self.C+1)]
        print(X,"所属的类别为:",time.index(max(time))+1)
        return time.index(max(time))+1
    #方法2：对计算得到的距离数组建最大堆
    def max_heap_scan(self,X):
        dis = [self.distance(X, i) for i in self.data]
        self.buildHeap(dis)
        Class=self.label[0:self.K]
        time = [Class.count(i) for i in range(1, self.C + 1)]
        print(X, "所属的类别为:", time.index(max(time)) + 1)
        return time.index(max(time)) + 1
