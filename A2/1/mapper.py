#!/usr/bin/env python3

import sys
import numpy as np


def get_matrix():
    '''
    input a matrix of shape (r, c) from STDIN (standard input)
    '''
    r, c = [int(x) for x in sys.stdin.readline().strip().split()]
    M = np.zeros((r, c))

    for i in range(r):
        row = sys.stdin.readline().strip()
        M[i] = np.array([int(v) for v in row.split()])

    return r, c, M


m, n, A = get_matrix()
n, k, B = get_matrix()

# printing key-value pairs to STDOUT
for a in range(m):
    for c in range(k):
        for b in range(n):
            print("%s\t%s" % ((a, c, b), (int(A[a][b]))))
            print("%s\t%s" % ((a, c, b), (int(B[b][c]))))
