#include "spline.h"

#include <stdio.h>
void Spline::FindMoment(int *start, int *finish, double moment)
{
    if (moment == time[0])
    {
        *start = 0;
        *finish = 1;
    }
    else if (moment == time[time.size() - 2])
    {
        *start = time.size() - 3;
        *finish = time.size() - 2;
    }
    else
    {
        for (int i = 0; *start == -1 && *finish == -1 && i < time.size() - 1; i++)
        {
                if (time[i] <= moment && moment <= time[i + 1])
                {
                    *start = i;
                    *finish = i + 1;
                }
        }
    }
}

void Spline::MakeCoefficientC(std::vector<Coefficients>* splineValues, std::vector<double> angles)
{
    std::vector<std::pair<double, double> > goCoeff;
    double A, B, D, F;

    for (int i = 0; i < time.size(); i++)
        goCoeff.push_back(std::make_pair(0,0));

    for (int i = 2; i < time.size() - 1; i++)//-1
    {
        A = time[i - 1] - time[i - 2];
        B = -2 * (time[i] - time[i - 2]);
        D = time[i] - time[i - 1];
        F = 3*(-(angles[i] - angles[i - 1])/(time[i] - time[i - 1]) + (angles[i - 1] - angles[i - 2])/(time[i - 1] - time[i - 2]));

        goCoeff[i].first = D/(B - A * goCoeff[i - 1].first);
        goCoeff[i].second = (F + A * goCoeff[i - 1].second)/(B - A * goCoeff[i - 1].first);
    }

    (*splineValues)[time.size() - 1].c = 0;//-1

    for (int i = splineValues->size() - 2; i > 1; i--)//-2
    {
        A = time[i - 1] - time[i - 2];
        B = -2 * (time[i] - time[i - 2]);
        D = time[i] - time[i - 1];
        F = 3*(-(angles[i] - angles[i - 1])/(time[i] - time[i - 1]) + (angles[i - 1] - angles[i - 2])/(time[i - 1] - time[i - 2]));

        (*splineValues)[i].c = goCoeff[i + 1].first * (*splineValues)[i + 1].c + goCoeff[i + 1].second;
    }
    goCoeff.clear();
}

double Spline::MakeCoefficientB(std::vector<Coefficients> splineValues, std::vector<double> angles, int i)
{
    double h = time[i] - time[i - 1];
    return ((angles[i] - angles[i - 1])/h) - h*(splineValues[i + 1].c + 2 * splineValues[i].c)/3;
}

void Spline::CreateCoefficients(std::vector<Coefficients>* splineValues, std::vector<double> angles)
{
    for(int i = 0; i < time.size(); i++)
    {
        Coefficients newCoeff;
        newCoeff.a = 0;
        newCoeff.b = 0;
        newCoeff.c = 0;
        newCoeff.d = 0;
        splineValues->push_back(newCoeff);
    }
    MakeCoefficientC(splineValues, angles);

    for (int i = 1; i < time.size() - 1; i++) //-1
    {
        (*splineValues)[i].a = angles[i - 1];
        (*splineValues)[i].b = MakeCoefficientB((*splineValues), angles, i);
        (*splineValues)[i].d = ((*splineValues)[i + 1].c - (*splineValues)[i].c)/(3*(time[i] - time[i - 1]));
    }
}

Spline::Spline()
{
    isWiden = false;
}

void Spline::Clear()
{
    if (angleX.size())
        angleX.clear();
    if (angleY.size())
        angleY.clear();
    if (angleZ.size())
        angleZ.clear();
    if (time.size())
        time.clear();
    if (splineX.size())
        splineX.clear();
    if (splineY.size())
        splineY.clear();
    if (splineZ.size())
        splineZ.clear();
}

void Spline::AddAngleTime(double x, double y, double z, double moment)
{
    angleX.push_back(x);
    angleY.push_back(y);
    angleZ.push_back(z);
    time.push_back(moment);
}

void Spline::MakeCoefficients()
{
    if (!isWiden)
    {
        time.push_back(0);
        angleX.push_back(0);
        angleY.push_back(0);
        angleZ.push_back(0);
        isWiden = true;
    }
    CreateCoefficients(&splineX, angleX);
    CreateCoefficients(&splineY, angleY);
    CreateCoefficients(&splineZ, angleZ);
}

TransformValues Spline::GetAngles(double moment)
{
    int start = -1;
    int finish = -1;
    double delta = 0;
    TransformValues rotation;
    FindMoment(&start, &finish, moment);
    delta = moment - time[start];
    rotation.paramOne = splineX[finish].a + splineX[finish].b * delta + splineX[finish].c * delta * delta + splineX[finish].d * delta * delta *delta;
    rotation.paramTwo = splineY[finish].a + splineY[finish].b * delta + splineY[finish].c * delta * delta + splineY[finish].d * delta * delta *delta;
    rotation.paramThree = splineZ[finish].a + splineZ[finish].b * delta + splineZ[finish].c * delta * delta + splineZ[finish].d * delta * delta *delta;
    rotation.type = ROTATE;
    return rotation;
}
