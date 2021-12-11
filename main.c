#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "matrixutil.h"
#include "fox.h"
#include <math.h>

#define ROOT_PRECESS 0

int main(int argc, char **argv)
{
    int numberOfPrecesses = 2;
    int rank = 0;

    int size;
    unsigned int q;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numberOfPrecesses); // return nr of precesses in this comunicator
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);              // returns the ranks of the curent process in a given comunicator

    FILE *file = stdin;

    MPI_Datatype matrixReicive;

    if (rank == 0)
    {
        fscanf(file, "%d", &size); //also scanable from the terminal
    }

    MPI_Bcast(&size, 1, MPI_INT32_T, ROOT_PRECESS, MPI_COMM_WORLD);

    unsigned int *matrix = NULL;

    alloc_matrix(size, &matrix);

    if (rank == 0)
    {
        double Q = sqrt(numberOfPrecesses);

        if (Q != (int)Q)
        {
            MPI_Finalize();
            free(matrix);
            return 0;
        }
        else if (size % (int)Q != 0)
        {
            MPI_Finalize();
            free(matrix);
            return 0;
        }
        else
        {
            readMatrix(file, matrix, size, numberOfPrecesses);
        }
    }

    //printf("Got out of inital rank 0 config. Process nr: %d\n", rank);

    MPI_Bcast(&q, 1, MPI_UINT32_T, ROOT_PRECESS, MPI_COMM_WORLD);

    GRID_INFO_TYPE info;

    setupGrid(&info);
    MPI_Type_vector(size / info.q, size / info.q, size / info.q, MPI_UINT32_T, &matrixReicive);
    MPI_Type_commit(&matrixReicive);

    unsigned int *localA = NULL, *localB = NULL, *localC = NULL;

    int smaller_matrix_size = size / info.q;

    alloc_matrix(smaller_matrix_size, &localA);
    alloc_matrix(smaller_matrix_size, &localB);
    alloc_matrix(smaller_matrix_size, &localC);

    MPI_Scatter(matrix, smaller_matrix_size * smaller_matrix_size, MPI_INT, localA, smaller_matrix_size * smaller_matrix_size, MPI_INT, ROOT_PRECESS, info.comm);

    matrix_copy(smaller_matrix_size, localB, localA);

    allPairs(size, &info, localA, localB, localC);

    MPI_Gather(localC, smaller_matrix_size * smaller_matrix_size, MPI_INT, matrix, smaller_matrix_size * smaller_matrix_size, MPI_INT, ROOT_PRECESS, info.comm);

    if (info.my_rank == ROOT_PRECESS)
    {
        printf("------ANSWER------\n");
        printMatrix(matrix, size, numberOfPrecesses);
    }

    free_matrix(&matrix);
    free_matrix(&localA);
    free_matrix(&localB);
    free_matrix(&localC);

    MPI_Finalize();

    return 0;
}
