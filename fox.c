#include "mpi.h"
#include "matrixutil.h"
#include "fox.h"
#include <math.h>

unsigned int setupGrid(GRID_INFO_TYPE *grid)
{

    unsigned int oldRank;
    unsigned int dimensions[2];
    unsigned int periods[2];
    unsigned int coordinates[2];
    unsigned int varying_coords[2];

    //Get the number of processes
    MPI_Comm_size(MPI_COMM_WORLD, &(grid->p));
    //Get the rank of the given process
    MPI_Comm_rank(MPI_COMM_WORLD, &oldRank);

    grid->q = (unsigned int)sqrt((double)grid->p);

    dimensions[0] = dimensions[1] = grid->q;

    periods[0] = periods[1] = 1;

    //Creating a new comunicator with two dimentons: Rows and columns
    //pereiods var indicating that dimentions are periodical n-1 comunicates with 0
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 1, &(grid->comm));

    //Get the rank of a given process in the new comunicator
    MPI_Comm_rank(grid->comm, &grid->my_rank);

    //Get the coordinates of a given process in the grid
    MPI_Cart_coords(grid->comm, grid->my_rank, 2, coordinates);

    grid->my_row = coordinates[0];
    grid->my_col = coordinates[1];

    //Setting up the comunicator for the rows. Sincw we want to vary the column but
    //not the row this will transfor the coordinates in for ex: (2,1) => (1)
    varying_coords[0] = 0;
    varying_coords[1] = 1;

    MPI_Cart_sub(grid->comm, varying_coords, &grid->row_comm);

    //Setting up the comunicator for the columns. Sincw we want to vary the row but
    //not the column this will transfor the coordinates in for ex: (2,1) => (2)
    varying_coords[0] = 1;
    varying_coords[1] = 0;

    MPI_Cart_sub(grid->comm, varying_coords, &grid->col_comm);

    return 1;
}

void Fox(unsigned int n, GRID_INFO_TYPE *grid,
         unsigned int *local_A,
         unsigned int *local_B,
         unsigned int *local_C)
{

    unsigned int *temp_A = NULL;
    unsigned int step;
    unsigned int bcast_root;
    unsigned int n_bar = n / grid->q;
    unsigned int source;
    unsigned int dest;
    unsigned int tag = 43;
    MPI_Status status;

    alloc_matrix(n_bar, &temp_A);
    fill_matrix(n_bar, temp_A, 0);

    source = (grid->my_row + 1) % grid->q;
    dest = (grid->my_row + grid->q - 1) % grid->q;

    for (step = 0; step < grid->q; step++)
    {
        bcast_root = (grid->my_row + step) % grid->q;

        if (bcast_root == grid->my_col)
        {
            //printf("local a bcast\n");
            MPI_Bcast(local_A, n_bar * n_bar, MPI_INT, bcast_root, grid->row_comm);
            //printf("local a bcast passed\n");
            matrix_nultiply(n_bar, local_A, local_B, local_C);
            //printf("finished multiplying\ local\n");
        }
        else
        {
            //printf("temp a bcast\n");
            MPI_Bcast(temp_A, n_bar * n_bar, MPI_INT, bcast_root, grid->row_comm);
            //printf("temp a bcast passed\n");
            matrix_nultiply(n_bar, temp_A, local_B, local_C);
            //printf("finished multiplying temp\n");
        }

        MPI_Sendrecv_replace(local_B, n_bar*n_bar, MPI_INT, dest, tag, source, tag, grid->col_comm, &status);
    }

    free_matrix(&temp_A);
}

void allPairs(unsigned int matrixSize, GRID_INFO_TYPE *grid, unsigned int *localA, unsigned int *localB, unsigned int *localC)
{
    unsigned int m = 1;

    //printf("Not Filled %d %d\n", matrixSize, grid->q);

    fill_matrix(matrixSize / grid->q, localC, __INT_MAX__ - 1);

    //printf("Filled\n");

    while (m < matrixSize - 1)
    {
        Fox(matrixSize, grid, localA, localB, localC);

        //printf("%d\n", m);

        matrix_copy(matrixSize / grid->q, localA, localC);
        matrix_copy(matrixSize / grid->q, localB, localC);

        m *= 2;
    }
}


//}