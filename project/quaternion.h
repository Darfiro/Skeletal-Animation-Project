#ifndef QUATERNION_H
#define QUATERNION_H

#include <stdlib.h>
#define FOUR 4

class Quaternion
{
    private:
        double** transformMatrix;

        void MakeSmaller(double** matrix, double **result, int n, int i, int j);
        double DeterminantRow(double **matrix, int n);
        void GetAdjoint(double **matrix, double **adj);

    public:

        double** AllocMatrix(int n, int m);
        void FreeMatrix(double** matrix, int n);

        Quaternion();

        double** GetMatrix();
        void MultMatrix(double** parentMatrix);
        void CopyMatrix(double** matrix);
        double** Inverse();

        ~Quaternion();

};

#endif // QUATERNION_H
