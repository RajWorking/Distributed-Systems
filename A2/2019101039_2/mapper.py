#!/usr/bin/env python3

import sys
import numpy as np

cluster = np.arange(101)

def find_cluster(x):
    if x == cluster[x]:
        return x
    cluster[x] = find_cluster(cluster[x])
    return cluster[x]

def merge_cluster(x, y):
    u = find_cluster(x)
    v = find_cluster(y)
    
    if u == v: # belong to same cluster
        return
    elif u > v:
        u, v = v, u # swap u and v

    cluster[v] = u

for line in sys.stdin:
    line = [int(x) for x in line.strip().split()]
    line.sort()
    i, j = line
    merge_cluster(i, j)
    
for node in range(1, 101):
    cluster[node] = find_cluster(node) # unification process
    print("%s\t%s" % (cluster[node], node))
