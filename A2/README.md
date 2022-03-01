# Assignment 2
```
Raj Maheshwari
2019101039
```
______________________________________

## Question 1:

#### Mapper:
We need to perform matrix multiplication of 2 matrices.  
matrix A have dimension - m x n,  
matrix B have dimension - n x p

Thus C = A @ B has dimension = m x p

now, each element of matrix C requires the sum of the `n` corresponding products of elements in A and B.  
Thus, for every  `(i, k)` element, we need `0..n-1` values of the row and column in A and B respectively. Generating the key-value pairs -->  
```
(i, k, j)  ==> A[i][j]
(i, k, j)  ==> B[j][k]
```

#### Reducer:
Upon reciving the key-value pairs from the mapper, we multiply these adjacent values and take the sum of these pairwise products. This gives us the value at the `(i, k)` index of the C matrix.

______________________________________

## Question 2:

#### Mapper:
Since we need to find connected components, we can use Disjoint set union algorithm. For each edge, we marge the parents of the 2 clusters to which the nodes belong. Then, we find the root node for each of the 100 nodes. Finally, the key-value pairs are published where the root node of each component is the key and the nodes in that connected component are the values.

#### Reducer:
The reducer simply displays the values corresponding to the same key as belonging to the same component. Since key-value pairs are sorted, the nodes of each component are consecutive.
