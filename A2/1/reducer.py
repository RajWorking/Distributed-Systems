#!/usr/bin/env python3

import sys
import numpy as np

''' computing C = A @ B  '''

# ith row of A, kth column of B, jth index in both
# results in C[i][k]
cur_pos = {'i': 0, 'j': 0, 'k': 0}

row = []
cur_val = 0
cur_sum = 0


for line in sys.stdin:
    line = line.strip()
    key, value = line.split('\t')
    i, k, j = [int(v.strip('()')) for v in key.split(',')]
    val = int(value)

    if j == cur_pos['j']:
        cur_sum += val * cur_val  # adding A[i][j] * B[j][k] to current sum

    # finalising value when changing column
    if k != cur_pos['k']:
        row += [cur_sum]  # appending C[i][k] to ith row of C
        cur_sum = 0  # reinitialising cur_sum

    # finalising values when changing row
    if i != cur_pos['i']:
        print(row)  # output C[i] by printing to STDOUT
        row = []  # reinitialising row

    cur_pos = {'i': i, 'j': j, 'k': k}  # caching the current values of i, j, k
    cur_val = val  # caching current value A[i][j]

row += [cur_sum]  # last element of matrix appended to last row
print(row)  # output last row
