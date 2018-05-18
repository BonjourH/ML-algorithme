import numpy as np
class Node:
    def __init__(self,data,label,split,left=None,right=None):
        self.data=data
        self.label=label
        #data数据形式为：一行=[x1,x2,...,xn,label]
        self.split=split
        self.left=left
        self.right=right
class Kd_Tree:
    def __init__(self,root=None):
        self.root=root
    def creat(self,data):
        #如果data为空
        if len(data)==0:
            return
        #寻找分割域
        Var=[np.var(data,axis=i) for i in range(data[0]-1)]
        index=Var.index(max(Var))
        #对data进行分组
        data=data[data[:,index].argsort()]#按列的大小对行进行排序
        value=data[index,:]
        root=Node(data=value[:-1],split=index,label=value[-1])
        left=data[1:index,:]
        right=data[index+1:len(data),:]
        root.left=self.creat(left)
        root.right = self.creat(right)
        return root

class K_nearest(Kd_Tree):
    def __init__(self, _k=0, _C=0, _data=[], _label=[]):
        super().__init__()
        self.K = _k  # K个近邻
        self.C = _C  # C个类别
        self.data = _data
        self.label = _label
    def distance(self,a,b):
        a=np.array(a)
        b=np.array(b)
        return np.sqrt((a-b).dot(a-b))

    def constructPath(self,target, tree, path_deque):
        # 传入参数分别为目标向量，训练集kd树，路径栈path

        while tree:
            # 遍历树的过程
            path_deque.append(tree)
            split = tree.split

            if target[split] < tree.point[split]:
                tree = tree.left
            else:
                tree = tree.right

        return path_deque

    def search(self,root, k, X, C):
        #我们假设数据总量大于k值
        # 建立初始路径
        track = self.constructPath(X, root, [])
        min_k = []
        dis = []
        while track:
            # 回溯
            temp_root = track.pop()
            d = self.distance(X, temp_root)
            if len(dis) < k or max(dis) > d:
                if len(dis) == k:
                    index = dis.index(max(dis))
                    dis.pop(index)
                    min_k.pop(index)
                min_k.append(temp_root)
                dis.append(d)
            # 验证当前节点是否满足H(X,node,min)条件或者还没k个点
            split = temp_root.split
            if abs(X[split] - temp_root.data[split]) < max(dis) or len(dis) < k:
                if X[split] <= temp_root.data[split]:
                    self.constructPath(X, temp_root.right, track)
                else:
                    self.constructPath(X, temp_root.left, track)
        label = [min_k[i].label for i in range(len(min_k))]
        # 各个类出现的次数
        time = [label.count(i) for i in range(1, C + 1)]
        return time.index(max(time)) + 1
