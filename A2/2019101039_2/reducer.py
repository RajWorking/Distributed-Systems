#!/usr/bin/env python3

import sys
import numpy as np

cluster = []
cur_cluster = 1

for line in sys.stdin:
    line = line.strip()
    key, value = [int(x) for x in line.split('\t')]

    if key != cur_cluster:
        cluster.sort()
        print(*cluster, sep=' ')        
        cur_cluster = key # reinitialising
        cluster = [value]
        
    else:
        cluster += [value]

print(*cluster, sep=' ')
