#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QColor>
#include "point3d.h"
#include <vector>
#include<bits/stdc++.h>

#define VERTEXCOUNT 3

struct Intersaction
{
    int y;
    std::pair<int, double> start;
    std::pair<int, double> finish;
};

class Triangle
{
    private:
        QColor color;
        std::vector<Point3D*> vertex;//[VERTEXCOUNT];
        std::vector<Intersaction> foundIntersactions;

        void ExchangeVerteces(int i, int j);

        int findY(int y);

    public:
        Triangle();

        void ClearRast();

        void SetVertices(Point3D* firstVertex, Point3D* secondVertex, Point3D* thirdVertex);
        void SetColor(QColor triangleColor);
        void SetPoint(int number, Point3D* point);
        void Clear();

        Point3D* GetPoint(int number);
        QColor& GetColor();
        std::vector<Point3D*> GetTriangle();

        // z буффер
        void Sort();
        void GetMaxZ(double *z);
        Intersaction GetIntersaction(int y);
        void LineRast(double x1, double y1, double z1, double x2, double y2, double z2);

        ~Triangle();
};

#endif // TRIANGLE_H
