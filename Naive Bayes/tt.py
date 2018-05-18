import numpy as np
a=[
    [4,2,3,4],
    [5,7,6,5],
    [1,10,-2,6]
]
print(len(a))
a=np.array(a)
b=np.argsort(a[:,0])
print(a[b])
c=[1,2,3,4,5]
print(c[:-1])
print(abs(-0.4))