#ifndef SPLINE_H
#define SPLINE_H

#include <vector>
#include <math.h>
#include "transformvalues.h"

struct Coefficients
{
    double a;
    double b;
    double c;
    double d;
};

class Spline
{
    private:

        std::vector<double> angleX, angleY, angleZ;                 // углы для каждого моменты времени
        std::vector<double> time;                   // моменты времени
        std::vector<Coefficients> splineX, splineY, splineZ;     // сплайн
        bool isWiden;                               // считались ли уже коэффициенты

        // Нахождение нужного промежутка
        void FindMoment(int *start, int *finish, double moment);

        // Коэффициент с
        void MakeCoefficientC(std::vector<Coefficients>* splineValues, std::vector<double> angles);

        // Коэффициент b
        double MakeCoefficientB(std::vector<Coefficients> splineValues, std::vector<double> angles, int i);

        // Вычисление коэффициентов для данной оси
        void CreateCoefficients(std::vector<Coefficients>* splineValues, std::vector<double> angles);

    public:
        Spline();

        void Clear();

        // Инициализация
        void AddAngleTime(double x, double y, double z, double moment);

        // Создание коэффициентов
        void MakeCoefficients();

        // Интерполяция
        TransformValues GetAngles(double moment);
};

#endif // SPLINE_H
