#include "quaternion.h"

void Quaternion::MakeSmaller(double** matrix, double **result, int n, int i, int j)
{
    int x = -1 , y = -1;
    for (int k = 0; k < n; k++)
    {
        if (k != i)
        {
            x += 1;
            for (int h = 0; h < n; h++)
            {
                if (h != j)
                {
                    y += 1;
                    result[x][y] = matrix[k][h];
                }
            }
            y = -1;
        }
    }
}

double Quaternion::DeterminantRow(double **matrix, int n)
{
    double det = 0;
    double **temp;
    int k = 1;
    temp = AllocMatrix(n, n);


    if (n == 1)
        det = matrix[0][0];
    else if (n == 2)
        det = matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1];
    else
    {
        for (int i = 0; i < n; i++)
        {
            MakeSmaller(matrix, temp, n, 0, i);
            det += k * matrix[0][i] * DeterminantRow(temp, n - 1);
            k *= -1;
        }
    }

    free(temp);
    return det;
}

void Quaternion::GetAdjoint(double **matrix, double **adj)
{
    int sign = 1;
    double** temp = AllocMatrix(FOUR, FOUR);
    double det;

    for (int i = 0; i< FOUR; i++)
    {
        for (int j = 0; j< FOUR; j++)
        {
            MakeSmaller(matrix, temp, FOUR, i, j);
            sign = ((i+j) % 2 == 0) ? 1: -1;

            det = DeterminantRow(temp, FOUR - 1);
            adj[j][i] = (sign)*(det);
        }
    }
    FreeMatrix(temp, FOUR);
}

double **Quaternion::AllocMatrix(int n, int m)
{
    double **res;
    res = (double**)calloc(n, sizeof(double*));
    for(int i = 0; i < n; i++)
    {
        res[i] = (double*)calloc(m, sizeof(double));
    }
    return res;
}

void Quaternion::FreeMatrix(double** matrix, int n)
{
    for(int i = 0;i < n; i++)
        free(matrix[i]);
    free(matrix);
}

Quaternion::Quaternion()
{
    transformMatrix = AllocMatrix(FOUR, FOUR);
}

double** Quaternion::GetMatrix()
{
    return transformMatrix;
}

void Quaternion::MultMatrix(double **parentMatrix)
{
    double** result = AllocMatrix(FOUR, FOUR);
    for (int i = 0; i < FOUR; i++)
    {
        for (int j = 0; j < FOUR; j++)
        {
            result[i][j] = 0;
            for (int r = 0; r < FOUR; r++)
                result[i][j] += parentMatrix[i][r] * transformMatrix[r][j];
        }
    }
    CopyMatrix(result);
    FreeMatrix(result, FOUR);
}

void Quaternion::CopyMatrix(double **matrix)
{
    for (int i = 0; i < FOUR; i++)
        for (int j = 0; j < FOUR; j++)
        {
            int temp = (int)(matrix[i][j]*1000);
            transformMatrix[i][j] = temp/1000.0;
        }
}

double **Quaternion::Inverse()
{
    double determinant = DeterminantRow(transformMatrix, FOUR);

    double **adj = AllocMatrix(FOUR, FOUR);
    double **result = AllocMatrix(FOUR, FOUR);
    GetAdjoint(transformMatrix, adj);

    for (int i = 0; i < FOUR; i++)
        for (int j = 0; j < FOUR; j++)
            result[i][j] = adj[i][j]/determinant;

    FreeMatrix(adj, FOUR);
    return result;
}

Quaternion::~Quaternion()
{
    FreeMatrix(transformMatrix, FOUR);
}
