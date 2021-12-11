#include "memory.h"
#include <stdlib.h>
#include <stdio.h>
#include "matrixutil.h"
#include <math.h>

int alloc_matrix(unsigned int size, unsigned int **matrix)
{

    if (*matrix != NULL)
    {
        return 0;
    }

    *matrix = (unsigned int *)malloc(size * size * sizeof(unsigned int));
    return 1;
}

int matrix_copy(unsigned int size, unsigned int *dest, unsigned int *src)
{

    if (dest == NULL || src == NULL)
    {

        return 0;
    }

    memcpy(dest, src, size * size * sizeof(unsigned int));
    return 1;
}

int free_matrix(unsigned int **matrix)
{

    if (*matrix != NULL)
    {
        free(*matrix);
    }
    else
    {
        return 0;
    }

    *matrix = NULL;
    return 1;
}

int fill_matrix(unsigned int matrixSize, unsigned int *matrix, unsigned int value)
{

    //printf("%d %d value\n", matrixSize);

    if (matrix == NULL)
    {
        return 0;
    }

    for (int i = 0; i != matrixSize * matrixSize; i++)
    {
        matrix[i] = value;
    }

    return 1;
}

int matrix_nultiply(unsigned int matrixSize, unsigned int *matrixA, unsigned int *matrixB, unsigned int *matrixC)
{

    for (int i = 0; i < matrixSize; i++)
    {
        for (int j = 0; j < matrixSize; j++)
        {
            for (int k = 0; k < matrixSize; k++)
            {
                unsigned int valueC = matrixC[i * matrixSize + j];
                unsigned int valueAB = matrixA[i * matrixSize + k] + matrixB[k * matrixSize + j];

                if (valueC > valueAB)
                {
                    matrixC[i * matrixSize + j] = valueAB;
                }
                else
                {
                    matrixC[i * matrixSize + j] = valueC;
                }
            }
        }
    }

    return 1;
}

int readMatrix(FILE *fp, unsigned int *matrix, int mSize, int p)
{

    int x = 0, i = 0, j = 0, size = mSize * mSize, new_size = size / p;
    int new_n = (int)sqrt((double)new_size);
    int q = (int)sqrt((double)p);

    for (int col = 0; col < q; col++)
    {
        for (int or = 0; or < new_n; or ++)
        {
            for (int row = 0; row < q; row++)
            {
                for (int o = 0; o < new_n; o++)
                {
                    int aux;
                    fscanf(fp, "%d", &aux);
                    if (aux == 0 && i != j)
                        matrix[x] = __INT_MAX__;
                    else
                    {
                        printf("%d %d\n", aux, x);
                        matrix[x] = aux;
                    }
                    x++;
                    if (j + 1 == mSize)
                    {
                        j = 0;
                        i++;
                    }
                    else
                        j++;
                }
                if (row + 1 != q)
                    x += new_size - new_n;
            }
            if (or +1 != new_n)
                x -= ((q - 1) * new_size);
        }
    }
    return 1;
}

void printMatrix(unsigned int *matrix, int mSize, int p)
{
    int x = 0, i = 0, j = 0, size = mSize * mSize, new_size = size / p;
    int new_n = (int)sqrt((double)new_size);
    int q = (int)sqrt((double)p);

    for (int col = 0; col < q; col++)
    {
        for (int or = 0; or < new_n; or ++)
        {
            for (int row = 0; row < q; row++)
            {
                for (int o = 0; o < new_n; o++)
                {
                    int aux = matrix[x];
                    if (aux == __INT_MAX__ - 1)
                        printf("0");
                    else
                        printf("%d", aux);
                    x++;
                    if (j + 1 == mSize)
                    {
                        j = 0;
                        i++;
                        printf("\n");
                    }
                    else
                    {
                        j++;
                        printf(" ");
                    }
                }
                if (row + 1 != q)
                    x += new_size - new_n;
            }
            if (or +1 != new_n)
                x -= ((q - 1) * new_size);
        }
    }
}
