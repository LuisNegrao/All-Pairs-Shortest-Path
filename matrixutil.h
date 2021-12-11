#include <stdio.h>

int alloc_matrix(unsigned int size, unsigned int **matrix);
int matrix_copy(unsigned int size, unsigned int *dest, unsigned int *src);
int free_matrix(unsigned int **max);
int fill_matrix(unsigned int matrixSize, unsigned int *matrix, unsigned int value);
int matrix_nultiply(unsigned int matrixSize, unsigned int *matrixA, unsigned int *matrixB, unsigned int *matrixC);
int readMatrix(FILE *fp, unsigned int *matrix, int mSize, int p);
void printMatrix(unsigned int *matrix, int mSize, int p);