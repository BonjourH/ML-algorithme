from collections import deque
import numpy as np
#最近邻搜索算法
def calcDistance(a,b):
        a=np.array(a)
        b=np.array(b)
        return np.sqrt((a-b).dot(a-b))

def constructPath(target,tree,path_deque):
    #传入参数分别为目标向量，训练集kd树，路径栈path

    while tree:
        #遍历树的过程
        path_deque.append(tree)
        split = tree.split

        if target[split] < tree.point[split]:
            tree = tree.left
        else:
            tree = tree.right

    return path_deque

def findNearestNeighbor(target,tree):

    path_deque = constructPath(target,tree,deque())
    #首先遍历训练集树，创建原始path栈
    kd_node = path_deque.pop()
    nearest_point = kd_node.point
    nearest_dist = calcDistance(target,nearest_point)
    #将nearest更新为叶结点

    while path_deque:
        #回溯
        kd_node = path_deque.pop()
        node_dist = calcDistance(target,kd_node.point)

        if node_dist < nearest_dist:
            nearest_point = kd_node.point
            nearest_dist = node_dist

        s = kd_node.split
        if abs(target[s] - kd_node.point[s]) < nearest_dist:
            #判断target与分割超
            if target[s] < kd_node.point[s]:
                path_deque = constructPath(target,kd_node.right,path_deque)
            else:
                path_deque = constructPath(target,kd_node.left,path_deque)


    return nearest_point,nearest_dist

def distance(a,b):
    pass
##K近邻搜索树
def search(root,k,X,C):
    #需要一个验证函数k是小于等于数据总数
    ## TO DO

    #建立初始路径
    track=constructPath(X,root,deque())
    min_k=[]
    dis=[]
    while track:
        #回溯
        temp_root=track.pop()
        d=distance(X,temp_root)
        if len(dis)<k or max(dis)>d:
            if len(dis)==k:
                index=dis.index(max(dis))
                dis.pop(index)
                min_k.pop(index)
            min_k.append(temp_root)
            dis.append(d)
        #验证当前节点是否满足H(X,node,min)条件或者还没k个点
        split=temp_root.split
        if abs(X[split]-temp_root.data[split])<max(dis) or len(dis)<k:
            if X[split]<=temp_root.data[split]:
                constructPath(X, temp_root.right, track)
            else:
                constructPath(X, temp_root.left, track)
    label=[min_k[i].label for i in range(min_k)]
    #各个类出现的次数
    time = [label.count(i) for i in range(1, C + 1)]
    return time.index(max(time))+1