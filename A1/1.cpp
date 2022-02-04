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

    // CODE STARTS

    int root_rank = 0;
    int n, sq;

    if (rank == root_rank)
    {
        freopen(argv[1], "r", stdin);
        cin >> n;
        sq = ceil(sqrt(n));
    }

    MPI_Bcast(&n, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    MPI_Bcast(&sq, 1, MPI_INT, root_rank, MPI_COMM_WORLD);

    bool divisor = false, notprime = false;
    for (int i = rank; i <= sq; i += numprocs)
    {
        if (i < 2)
            continue;
        divisor |= (n % i == 0);
    }

    MPI_Reduce(&divisor, &notprime, 1, MPI_C_BOOL, MPI_LOR, root_rank, MPI_COMM_WORLD);

    if (rank == root_rank)
    {
        freopen(argv[2], "w", stdout);
        cout << (notprime ? "NO" : "YES") << "\n";
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