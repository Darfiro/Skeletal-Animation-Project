#include "triangle.h"

#include <stdio.h>

Triangle::Triangle()
{
    Point3D *point = new Point3D;
    vertex.clear();
    vertex.push_back(point);
    vertex.push_back(point);
    vertex.push_back(point);
    color = Qt::white;
}

void Triangle::ClearRast()
{
    foundIntersactions.clear();
}

void Triangle::SetVertices(Point3D* firstVertex, Point3D* secondVertex, Point3D* thirdVertex)
{
    vertex[0] = firstVertex;
    vertex[1] = secondVertex;
    vertex[2] = thirdVertex;
}

void Triangle::SetColor(QColor triangleColor)
{
    color = triangleColor;
}

void Triangle::SetPoint(int number, Point3D* point)
{
    if (number >= 0 && number < 3)
    {        
        vertex[number] = point;
    }
}

void Triangle::Clear()
{
    vertex[0] = NULL;
    vertex[1] = NULL;
    vertex[2] = NULL;
    color = Qt::white;
}

Point3D* Triangle::GetPoint(int number)
{
    if (number > 0 && number < 3)
        return vertex[number];
}

QColor& Triangle::GetColor()
{
    return color;
}

std::vector<Point3D*> Triangle::GetTriangle()
{
    return vertex;
}

void Triangle::Sort()
{
    for (int i = 0; i < VERTEXCOUNT; i++)
    {
        for (int j = i + 1; j < VERTEXCOUNT; j++)
        {
            if (vertex[i]->GetY() > vertex[j]->GetY())
            {
                ExchangeVerteces(i, j);
            }
        }
    }
}

void Triangle::GetMaxZ(double *z)
{
    *z = vertex[0]->GetZ();
    for (int i = 1; i < VERTEXCOUNT; i++)
    {
        if (vertex[i]->GetX() > *z)
            *z = vertex[i]->GetZ();
    }
}

Intersaction Triangle::GetIntersaction(int y)
{
    Intersaction thatIntersaction;
    thatIntersaction.y = -1;
    for (size_t i = 0; i < foundIntersactions.size() && thatIntersaction.y == -1; i++)
    {
        if (y == foundIntersactions[i].y)
            thatIntersaction = foundIntersactions[i];
    }
    return thatIntersaction;
}

void Triangle::ExchangeVerteces(int i, int j)
{
    Point3D *point;
    point = vertex[i];
    vertex[i] = vertex[j];
    vertex[j] = point;
}

int Triangle::findY(int y)
{
    int found = -1;
    for (size_t i = 0; i < foundIntersactions.size() && found == -1; i++)
    {
        if (y == foundIntersactions[i].y)
            found = i;
    }
    return found;
}

void Triangle::LineRast(double x1, double y1, double z1, double x2, double y2, double z2)
{
    double l = 0, dx = 0, dy = 0, dz = 0;
    double x = x1, y = y1, z = z1, yt;

    if (fabs(x2 - x1) > fabs(y2 - y1))
        l = fabs(x2 - x1);
    else
        l = fabs(y2 - y1);
    dx = (x2 - x1)/l;
    dy = (y2 - y1)/l;
    dz = (z2 - z1)/l;
    yt = y;
    Intersaction newIntersaction;
    newIntersaction.y = int(round(y));
    newIntersaction.start = std::make_pair(round(x), z);
    newIntersaction.finish = std::make_pair(round(x), z);
    foundIntersactions.push_back(newIntersaction);

    while (fabs(x - x1) < fabs(x2 - x1) || fabs(y - y1) < fabs(y2 - y1))
    {
        x += dx;
        y += dy;
        z += dz;
        if (round(y) != round(yt))
        {
            int alreadyIn = findY(round(y));
            if (alreadyIn + 1)
            {
                if (int(round(x)) > foundIntersactions[alreadyIn].start.first && foundIntersactions[alreadyIn].start.first == foundIntersactions[alreadyIn].finish.first)
                {
                    foundIntersactions[alreadyIn].finish = std::make_pair(round(x), z);
                }
                else if (foundIntersactions[alreadyIn].start.first == foundIntersactions[alreadyIn].finish.first)
                {
                    foundIntersactions[alreadyIn].start = std::make_pair(round(x), z);
                }
            }
            else
            {
                Intersaction newIntersaction;
                newIntersaction.y = int(round(y));
                newIntersaction.start = std::make_pair(round(x), z);
                newIntersaction.finish = std::make_pair(round(x), z);
                foundIntersactions.push_back(newIntersaction);
            }
        }
        yt = y;
    }
}

Triangle::~Triangle()
{
    vertex[0] = NULL;
    vertex[1] = NULL;
    vertex[2] = NULL;
    color = Qt::white;
}


