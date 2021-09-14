#include "point3d.h"

#include <stdio.h>

Point3D::Point3D()
{
    x = 0;
    y = 0;
    z = 0;
}

Point3D::Point3D(double xAxis, double yAxis, double zAxis)
{
    x = xAxis;
    y = yAxis;
    z = zAxis;
}

void Point3D::SetPoint(Point3D &point)
{
    x = point.GetX();
    y = point.GetY();
    z = point.GetZ();
}

void Point3D::SetCoordinats(double xAxis, double yAxis, double zAxis)
{
    x = xAxis;
    y = yAxis;
    z = zAxis;
}

void Point3D::SetX(double xAxis)
{
    x = xAxis;
}

void Point3D::SetY(double yAxis)
{
    y = yAxis;
}

void Point3D::SetZ(double zAxis)
{
    z = zAxis;
}

void Point3D::SetClear()
{
    x = 0;
    y = 0;
    z = 0;
}

double& Point3D::GetX()
{
    return x;
}

double& Point3D::GetY()
{
    return y;
}

double& Point3D::GetZ()
{
    return z;
}

void Point3D::RotateDotX(double angle)
{
    double ty = y, tz = z;
    y = ty*cos(angle) - tz*sin(angle);
    z = ty*sin(angle) + tz*cos(angle);
}

void Point3D::RotateDotY(double angle)
{
    double tx = x, tz = z;
    x = tx*cos(angle) - tz*sin(angle);
    z = tx*sin(angle) + tz*cos(angle);
}

void Point3D::RotateDotZ(double angle)
{
    double tx = x, ty = y;
    x = tx*cos(angle) - ty*sin(angle);
    y = tx*sin(angle) + ty*cos(angle);
}

void Point3D::ResizeDot(double kx, double ky, double kz)
{
    x = x * kx;
    y = y * ky;
    z = z * kz;
}

void Point3D::MoveDot(double dx, double dy, double dz)
{
    x += dx;
    y += dy;
    z += dz;
}

Point3D::~Point3D()
{
    x = 0;
    y = 0;
    z = 0;
}

