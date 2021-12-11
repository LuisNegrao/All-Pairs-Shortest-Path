#include "mpi.h"

typedef struct
{

    unsigned int p;      // NUMBER OF PROCESSES
    MPI_Comm comm;       // COMMUNICATOR FOR THE GRID
    MPI_Comm row_comm;   // COMUNICATOR FOR THE ROW
    MPI_Comm col_comm;   // COMUNICATOR FOR THR COLUMN
    unsigned int q;      // ORDER OF THE GRID
    unsigned int my_row; // ROW NUMBER
    unsigned int my_col; // COLUMN NUMBER
    unsigned int my_rank; // RANK IN GRID COM

}GRID_INFO_TYPE;

unsigned int setupGrid(GRID_INFO_TYPE *grid);
void Fox(unsigned int n, GRID_INFO_TYPE *grid,
         unsigned int *local_A,
         unsigned int *local_B,
         unsigned int *local_C);
void allPairs(unsigned int matrixSize, GRID_INFO_TYPE *grid, unsigned int *localA, unsigned int *localB, unsigned int *localC);
