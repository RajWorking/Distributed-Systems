import numpy as np

a = np.array(
    [[36, 78, 55, 42, 2],
     [96, 12, 13, 42, 13],
     [75, 26, 34, 41, 198]]
)

b = np.array([
    [2],
    [10],
    [19]
]
)

print(b.T @ a)
