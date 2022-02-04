#include <iostream>
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
    int n;
    if (rank == root_rank)
    {
        freopen(argv[1], "r", stdin);
        cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, root_rank, MPI_COMM_WORLD);
    int nn = n * n;
    int adj[nn];
    if (rank == root_rank)
    {
        int a, b, w, m;
        cin >> m;
        memset(adj, -1, nn * sizeof(int));
        for (int i = 0; i < m; i++)
        {
            cin >> a >> b >> w;
            a--, b--;
            adj[n * a + b] = adj[n * b + a] = w;
        }
    }
    MPI_Bcast(&adj, nn, MPI_INT, root_rank, MPI_COMM_WORLD);

    int clique[11] = {0};
    int sum;

    for (int i = rank; i < n; i += numprocs)
        for (int ii = 0; ii < n; ii++)
            for (int ij = 0; ij < n; ij++)
            {
                if (ij == ii or adj[i * n + ii] == -1 or adj[i * n + ij] == -1 or adj[n * ii + ij] == -1)
                    continue;

                sum = adj[i * n + ii] + adj[i * n + ij] + adj[n * ii + ij];
                clique[sum]++;

                for (int ik = 0; ik < n; ik++)
                {
                    if (ik == ij or ik == ii or adj[i * n + ik] == -1 or adj[ik * n + ii] == -1 or
                        adj[ik * n + ij] == -1)
                        continue;

                    sum = adj[i * n + ii] + adj[i * n + ij] + adj[i * n + ik] + adj[n * ii + ij] + adj[ik * n + ii] +
                          adj[ik * n + ij];
                    clique[4 + sum]++;
                }
            }

    int final_ans[11] = {0};
    MPI_Reduce(&clique, &final_ans, 11, MPI_INT, MPI_SUM, root_rank, MPI_COMM_WORLD);

    if (rank == root_rank)
    {
        freopen(argv[2], "w", stdout);

        for (int i = 0; i < 4; i++)
            cout << "3 " << i << " " << (final_ans[i] / 6) << "\n";

        for (int i = 0; i < 7; i++)
            cout << "4 " << i << " " << (final_ans[4 + i] / 24) << "\n";
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