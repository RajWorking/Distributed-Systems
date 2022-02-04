# REPORT

## Alias
In order to make my life simpler, I wrote the following custom command with 2 arguments as the name of the program to run and the number of processes.
```bash
alias mpiall='_(){ mpicxx $1; mpirun -np $2 --oversubscribe ./a.out inp.txt out.txt; }; _'
$1: program file name
$2: number of processes
```

## General Comments

1. Since it is easier to share a 1D array amongst processes using functions like SEND, B_CAST, SCATTER etc., at all places a 2D matrix is represented in the following form:

   for a N * M matrix,
   `a[i][j]` is represented as `a[i * M + j]`

2. Input and Output are done from the file paths mentioned in the arguments. This is taken care of only by the root node, or process with rank id 0.
   

## Solutions concepts

### Question 1
The root node computes the square root of the input number N. The entire range of numbers from
`2 to sqrt(N)` needs to be searched for a divisor. This range is divided amongst the processes in a round robin or cyclic fashion. 
eg. 
```
P0 -> 0, 6, 12, 18 ....
P1 -> 1, 7, 13, 19 ....
P2 -> 2, 8, 14, 20 ....
.
.
P5 -> 5, 11, 17, 23 ....
```

Now, we take the logical OR of whether any of the processes have found atleast one divisor upto sqrt(N). If it is found then the number is NOT PRIME, else it is PRIME.

### Question 2

The algorithm for this question is to simply brute force finding all possible cliques in the graph.

For a 3 node clique, we go to each of the nodes and check if any 2 of the neighbours have an edge between them. Similarly, for a 4 node clique, we go to each node and check if 3 of its neighbours are connected to each other. 

Since this checking is independent amonst nodes, we can divide the starting nodes equally amongst the processes. Each process keeps its own count of the number of cliques of different weights. Then we sum up these counts for all the processes using MPI_REDUCE.

Every 3 node clique can have `3! = 6` possible permutations and every 4 node clique can have `4! = 24` possible permutations. Thus, we need to divide the counts respectively by 6 and 24.


### Question 3

The generic algorithm involves going over each row one by one and using the leading coefficient on its diagonal subtracting the corresponding ratio to zero the respective column. For each element on the diagonal, we need to iterate over the entire submatrix below it. This makes the complexity `O(n^3)`.

eg.
```
r1 = [3, 8, 3, 7 ...]
r2 = [6, 7, 9, 1 ...]

r2 - 2*r1 = [0, -9, 3, -13 ...]
```

In this question, each process get assigned a block of rows from the matrix. The parallel processing invloves 3 steps.
1. Receiving the different block of rows from each of the rows before it. Then using the rows for the subtraction process. 
2. The generic procedure is done for the rows within the block.
3. Finally, the updated block is sent down to all the processes after it.
   
Since the main computation part happens in step 2, the complexity reduces by a factor of `p^2` where p is the number of processes.  
But since each process needs to wait for the above processes to finish sending it their blocks, the sequential nature makes the overall complexity `O((n^3)/p)`