#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv)
{
    int rank, numprocs;

    // initiate MPI
    MPI_Init(&argc, &argv);

    // get size of the current communicator
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // get current process rank
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /*synchronize all processes*/
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // enter your code here
    // CODE STARTS

    int root_rank = 0;
    int n, m;
    if (rank == root_rank)
    {
        freopen(argv[1], "r", stdin);
        cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, root_rank, MPI_COMM_WORLD);

    m = n + 1;
    int mat_sz = n * m;
    int rows = ceil(n * 1.0 / numprocs);
    int chunk_size = rows * m;

    double mat[chunk_size * numprocs] = {0.0};
    double sub[chunk_size];
    double zero = 1e-6;

    if (rank == root_rank)
    {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                cin >> mat[i * m + j];
    }

    // each process gets a block section of consecutive `chunk_size` rows
    // each row has n+1 columns
    MPI_Scatter(mat, chunk_size, MPI_DOUBLE, sub, chunk_size, MPI_DOUBLE, root_rank, MPI_COMM_WORLD);

    // each process needs to wait for all the above blocks to finish processing.
    double recv[chunk_size];
    for (int i = 0; i < rank; i++)
    {
        MPI_Recv(recv, chunk_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (int j = 0; j < rows; j++) // jth recieved row
        {
            int mat_row = i * rows + j;            // row number in original matrix
            if (abs(recv[j * m + mat_row]) < zero) // to avoid run time error
                recv[j * m + mat_row] = zero;
            for (int k = 0; k < rows; k++) // kth row of its own sub matrix
            {
                double ratio = sub[k * m + mat_row] / recv[j * m + mat_row]; // ratio of leading elements
                for (int col = mat_row; col < m; col++)                      // going over all columns of row after diagonal
                    sub[k * m + col] -= ratio * recv[j * m + col];
            }
        }
    }

    // internal solving of block
    for (int j = 0; j < rows; j++)
    {
        int mat_row = rank * rows + j; // row number in original matrix
        if (mat_row >= n)              // only until last row
            break;
        if (abs(sub[j * m + mat_row]) < zero) // to avoid run time error
            sub[j * m + mat_row] = zero;

        for (int k = j + 1; k < rows; k++) // all rows below j
        {
            double ratio = sub[k * m + mat_row] / sub[j * m + mat_row]; // ratio of leading elements
            for (int col = mat_row; col < m; col++)                     // going over all columns of row after diagonal
                sub[k * m + col] -= ratio * sub[j * m + col];
        }
    }

    for (int i = rank + 1; i < numprocs; i++) // sending the submatrix to all processes below
        MPI_Send(sub, chunk_size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    double soln[n] = {0.0};
    MPI_Gather(sub, chunk_size, MPI_DOUBLE, mat, chunk_size, MPI_DOUBLE, root_rank, MPI_COMM_WORLD);

    // back substitution
    for (int i = n - 1; i >= 0; i--)
    {
        double sum = mat[i * m + n];
        for (int j = i + 1; j < n; j++)
            sum -= mat[i * m + j] * soln[j];

        if (abs(mat[i * m + i]) < zero)
            mat[i * m + i] = zero;

        soln[i] = sum / mat[i * m + i];
    }

    if (rank == root_rank)
    {
        freopen(argv[2], "w", stdout);
        for (int i = 0; i < n; i++)
            cout << setprecision(12) << soln[i] << " ";
        cout << "\n";
    }

    // CODE ENDS

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime() - start_time;
    double maxTime;
    // get max program run time for all processes
    MPI_Reduce(&end_time, &maxTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        cout << "Total time (s): " << maxTime << "\n";
    }

    // shut down MPI and close
    MPI_Finalize();
    return 0;
}